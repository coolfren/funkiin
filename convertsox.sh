#!/bin/bash
sox $1 -t raw -b 16 --endian big -c 2 $2