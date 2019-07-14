#!/bin/bash

for filename in test/*.cnf
	do
	test_out="$(./satSolver.o $filename)"
	echo "Testing $filename ...  $test_out"
done
