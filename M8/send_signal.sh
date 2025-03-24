#!/bin/bash

# Locate the child process PID using pgrep
CHILD_PID=$(pgrep -x child)

if [ -z "$CHILD_PID" ]; then
  echo "Child process not found."
  exit 1
fi

echo "Found child process with PID: $CHILD_PID"


for ((i=1; i<=5; i++)); do
  echo "Sending SIGTSTP to PID every 3 seconds $CHILD_PID "
  kill -SIGTSTP "$CHILD_PID"
  sleep 3
done

# Send SIGTERM to gracefully stop the process
echo "Sending SIGTERM to PID $CHILD_PID"
kill -SIGTERM "$CHILD_PID"

echo "Done."
