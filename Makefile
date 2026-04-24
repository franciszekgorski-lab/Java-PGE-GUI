SRC_DIR = src/java
RES_DIR = res
OUT_DIR = out
SRCS    = $(wildcard $(SRC_DIR)/*.java)
MAIN    = Main

.PHONY: all run clean

all: $(OUT_DIR)
	javac -d $(OUT_DIR) $(SRCS)
	cp -r $(RES_DIR)/* $(OUT_DIR)/

run: all
	java -cp $(OUT_DIR) $(MAIN)

clean:
	rm -rf $(OUT_DIR)

$(OUT_DIR):
	mkdir -p $(OUT_DIR)
