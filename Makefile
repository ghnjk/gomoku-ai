
all:
	mkdir -p target
	cp *.py target/
	mkdir -p target/data
	cd target ; tar -zcf gomoku.tgz ./

test:
	@echo "test ok"
	
clean:
	@find . -name "*.pyc" -exec rm {} \;
	@find . -name "*.DS_Store" -exec rm {} \;
	@rm -rf target
