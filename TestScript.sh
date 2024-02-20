#!/bin/bash
DBusService test /usr/bin/TestProg text/markdown ru.notepad /ru/notepad &
gdbus call --session \
    --dest ru.notepad \
    --object-path /ru/notepad \
    --method org.freedesktop.Application.Open "['README.md']" "{'DIR':<'/ApplicationRegistration'>}"


