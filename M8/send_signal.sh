#!/bin/bash

# Locate the child process PID using pgrep
CHILD_PID=$(pgrep -x child)

if [ -z "$CHILD_PID" ]; then
  echo "Child process not found."
  exit 1
fi

echo "Found child process with PID: $CHILD_PID"

# Number of SIGTSTP signals to send
NUM_SIGNALS=5
INTERVAL=3  # Time in seconds between signals

# Send SIGTSTP signals
for ((i=1; i<=NUM_SIGNALS; i++)); do
  echo "Sending SIGTSTP to PID every 3 seconds $CHILD_PID ($i/$NUM_SIGNALS)"
  kill -SIGTSTP "$CHILD_PID"
  sleep "$INTERVAL"
done

# Send SIGTERM to gracefully stop the process
echo "Sending SIGTERM to PID $CHILD_PID"
kill -SIGTERM "$CHILD_PID"

echo "Done."
