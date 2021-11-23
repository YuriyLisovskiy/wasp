FULL_BRANCH_NAME=$1

if [ "$FULL_BRANCH_NAME" = "master" ]; then
  echo "master"
else
  echo "dev"
fi
