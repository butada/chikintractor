#!/bin/bash

cd `dirname $0`

./run1.sh 2>&1 | tee -a run.log


