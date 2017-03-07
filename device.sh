#!/bin/bash

touch ext2device
dd if=/dev/zero of=ext2device bs=1024 count=102400
mkfs.ext2 ext2device