#!/bin/bash
CHILD_PID=$(pgrep -x child)

if [ -z "$CHILD_PID" ]; then
  echo "Child process not found."
  exit 1
fi
echo "Found child process with PID: $CHILD_PID"


for ((i=1; i<=5; i++)); do
  echo "Sending SIGTSTP to  $CHILD_PID "
  kill -SIGTSTP "$CHILD_PID"
  sleep 3
done

echo " Done sending signals. Now sending SIGTERM to child. $CHILD_PID"
kill -SIGTERM "$CHILD_PID"
