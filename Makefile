.PHONY: clean All

All:
	@echo "----------Building project:[ rmush2 - Release ]----------"
	@$(MAKE) -f  "rmush2.mk"
clean:
	@echo "----------Cleaning project:[ rmush2 - Release ]----------"
	@$(MAKE) -f  "rmush2.mk" clean
