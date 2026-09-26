#!/bin/sh
set -e

ISO=/kfs/kfs.iso

run_qemu() {
    exec qemu-system-i386 -drive file="$ISO",format=raw -vnc 0.0.0.0:0 -s $QEMU_FLAGS
}

run_bochs() {
    Xvfb :0 -screen 0 1024x768x24 &
    export DISPLAY=:0
    sleep 1
    x11vnc -display :0 -rfbport 5900 -localhost -forever -shared -nopw -quiet &
    exec bochs -q -f /kfs/bochsrc
}

novnc_server --vnc 127.0.0.1:5900 --listen 5800 &

EMU=${EMU:-qemu}
command -v "run_$EMU" >/dev/null 2>&1 || { echo "EMU inconnu : $EMU (qemu|bochs)" >&2; exit 1; }

"run_$EMU"
