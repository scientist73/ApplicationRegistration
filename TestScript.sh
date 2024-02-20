#!/bin/bash
export $(dbus-launch)
./DBusService test TestProg text/plain ru.notepad /ru/notepad &

gdbus call --session \
    --dest ru.notepad \
    --object-path /ru/notepad \
    --method org.freedesktop.Application.Open \
    "['text.txt']" "{'DIR':<'/ApplicationRegistration'>}"
