#!/bin/bash

touch dev
dd if=/dev/zero of=dev bs=1024 count=102400
mkfs.ext2 dev