.PHONY: clean All

All:
	@echo "----------Building project:[ booserver - Release ]----------"
	@cd "booserver" && "$(MAKE)" -f  "booserver.mk"
clean:
	@echo "----------Cleaning project:[ booserver - Release ]----------"
	@cd "booserver" && "$(MAKE)" -f  "booserver.mk" clean
