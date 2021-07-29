#include "./flag.h"

__CORE_FLAGS_BEGIN__

Flag::Flag(std::string shorthand, std::string name, std::string usage) :
	_shorthand(std::move(shorthand)), _name(std::move(name)), _usage(std::move(usage))
{
	if (this->_shorthand.empty())
	{
		throw ArgumentError("shorthand can not be empty", _ERROR_DETAILS_);
	}

	if (this->_shorthand.size() > 1)
	{
		throw ArgumentError(
			"'" + this->_shorthand + "' shorthand is more than one ASCII character", _ERROR_DETAILS_
		);
	}

	if (this->_shorthand.starts_with('-'))
	{
		throw ArgumentError("'" + this->_shorthand + "' shorthand should not start from -", _ERROR_DETAILS_);
	}

	if (this->_name.empty())
	{
		throw ArgumentError("name can not be empty", _ERROR_DETAILS_);
	}

	if (this->_name.starts_with('-'))
	{
		throw ArgumentError("'" + this->_name + "' name should not start from -", _ERROR_DETAILS_);
	}

	this->_shorthand = "-" + this->_shorthand;
	this->_name = "--" + this->_name;
}

__CORE_FLAGS_END__
