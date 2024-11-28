logPath=./build/tests/test_printer.log

./build/tests/test_printer |tee $logPath

python ./scripts-py/parse_chrome_tracing.py --inputlogfile $logPath --output ./test.json