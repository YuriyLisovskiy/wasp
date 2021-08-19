/**
 * http/multipart.cpp
 *
 * Copyright (c) 2021 Yuriy Lisovskiy
 */

#include "./multipart.h"

// C++ libraries.
#include <filesystem>

// Base libraries.
#include <xalwart.base/exceptions.h>


__HTTP_MULTIPART_BEGIN__

files::UploadedFile FileHeader::open() const
{
	if (this->content.empty())
	{
		return files::UploadedFile::from_path(this->tmpfile);
	}

	return files::UploadedFile::from_content(this->content);
}

void Form::remove_all() const
{
	for (const auto& fhs : this->files)
	{
		for (const auto& fh : fhs.second)
		{
			if (fh && !fh->tmpfile.empty())
			{
				std::filesystem::remove(fh->tmpfile);
			}
		}
	}
}

std::tuple<std::unique_ptr<Form>, ParseFormError> Reader::read_form(long long int max_memory)
{
	auto form = std::make_unique<Form>();

	// Reserve an additional 10 MB for non-file parts.
	auto max_value_bytes = max_memory + (10 << 20);
	while (true)
	{
		auto [p, err] = this->next_part();
		if (err == ParseFormError::Eof)
		{
			break;
		}

		if (err != ParseFormError::Nil)
		{
			return {nullptr, err};
		}

		auto name = p->form_name();
		if (name.empty())
		{
			continue;
		}

		std::string buffer;
		auto filename = p->file_name();
		if (filename.empty())
		{
			// value, store as string in memory
			auto [n, r_err] = p->read(buffer, max_value_bytes + 1);
			if (r_err != ParseFormError::Nil && r_err != ParseFormError::Eof)
			{
				return {nullptr, r_err};
			}

			max_value_bytes -= n;
			if (max_value_bytes < 0)
			{
				return {nullptr, ParseFormError::MessageTooLarge};
			}

			form->values.add(name, buffer);
			continue;
		}

		// file, store in memory or on disk
		auto fh = std::make_shared<FileHeader>();
		fh->filename = filename;
		fh->header = p->header;
		auto [n, r_err] = p->read(buffer, max_memory + 1);
		if (r_err != ParseFormError::Nil && r_err != ParseFormError::Eof)
		{
			return {nullptr, r_err};
		}

		if (n > max_memory)
		{
			// too big, write to disk and flush buffer
			auto size = n;
			auto file = path::temp_file("", "multipart-", true);
			if (file->is_open())
			{
				try
				{
					file->write(buffer);
					while (true)
					{
						auto [n_, err_] = p->read(buffer, max_memory + 1);
						size += n_;
						if (err_ == ParseFormError::Eof)
						{
							break;
						}

						if (err_ != ParseFormError::Nil)
						{
							return {nullptr, err_};
						}
					}

					file->save();
					file->close();
				}
				catch (const FileError&)
				{
					if (path::exists(file->path()))
					{
						std::filesystem::remove(file->path());
					}

					return {nullptr, ParseFormError::SaveFileFailure};
				}
			}

			fh->tmpfile = file->path();
			fh->size = size;
		}
		else
		{
			fh->content = buffer;
			fh->size = buffer.size();
			max_memory -= n;
			max_value_bytes -= n;
		}

		form->files.add(name, std::move(fh));
	}

	return {std::move(form), ParseFormError::Nil};
}

__HTTP_MULTIPART_END__
