.PHONY: clean All

All:
	@echo "----------Building project:[ IMU - Debug ]----------"
	@cd "IMU" && "$(MAKE)" -f  "IMU.mk"
clean:
	@echo "----------Cleaning project:[ IMU - Debug ]----------"
	@cd "IMU" && "$(MAKE)" -f  "IMU.mk" clean
