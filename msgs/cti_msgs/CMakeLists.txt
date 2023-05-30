cmake_minimum_required(VERSION 2.8.3)
project(cti_msgs)

find_package(catkin REQUIRED COMPONENTS
    message_generation
    actionlib_msgs
    geometry_msgs
    sensor_msgs
    nav_msgs
    std_msgs
)

add_message_files(
	FILES
		AutoTransmission.msg
		DistinguishCmd.msg
		state/BoxMountState.msg
		state/ChangeMapState.msg
		state/LiftRunState.msg
		state/LiftState.msg
		state/LiftRunTimeState.msg
		state/LocationState.msg
		state/MoveOutObstacleState.msg
		state/RobotNotifyState.msg
		state/RobotNotifyEvent.msg
		state/DockingState.msg
		state/NavigationControlState.msg
		state/MapManagerState.msg
		state/BaseState.msg
		state/BaseDebugState.msg
		state/BaseLedState.msg
		state/BaseLaserState.msg
		state/BaseLimitState.msg
		state/BaseMegState.msg
		state/BaseMotorState.msg
		state/BaseRemoteState.msg
		state/BaseUartQocState.msg
		state/BaseMiscSummary.msg
		state/BaseMotionAddState.msg
		state/SerialTxRxMonitor.msg
		state/PassagewayState.msg
		state/PassageDeviceState.msg
		state/SelfCheckBits.msg
		state/SubTaskState.msg
		state/WifiOperationState.msg
		state/SystemReboot.msg
		state/AbnormalSensor.msg
		
		wash/WashModeControl.msg
		wash/WashModeFeedback.msg
		wash/WorkModeControl.msg
		wash/WorkModeState.msg

		CloudElevatorWaypointInfo.msg
		CloudDockInfo.msg
		CloudGateInfo.msg
		CloudWaypointInfo.msg
		CloudElevatorInfo.msg
		CloudFloorInfo.msg
		CloudFloorZoneInfo.msg
		CloudFloorPathInfo.msg
		CloudBuildingInfo.msg
		CloudParkInfo.msg
		CloudRobotInfo.msg
		CloudTaskRequest.msg
		CloudTaskResponse.msg
		
		BuildingRobotState.msg
		MultiBuildingRobotState.msg
		BriefRobotState.msg
		
		BaseControl.msg
		BaseSensors.msg
		CarryBoxInfo.msg
		ElevatorCmd.msg
		ElevatorSensor.msg
		DistinguishState.msg
		NavigationControlCmd.msg
		
		NavigationMbfCtrlCmd.msg
		NavigationDeciderCmd.msg
		NavigationCmd.msg
		NetCardInfo.msg
		ChangeMapCmd.msg
		BoxMountCmd.msg
		VelocityParameter.msg
		# BuildingInfo.msg
		# FloorInfo.msg
		ChargingStationInfo.msg
		Range.msg
		RangeArray.msg
		RangeSimple.msg
		RangeSimpleArray.msg
		PolygonArray.msg
		PerceptionObstacle.msg
		PerceptionObstacleArray.msg
		JoyControl.msg
		ShiftRobotCmd.msg
		BaseUartSendCmd.msg
		SoundPlayCmd.msg
		TFminiData.msg
		TFminiDataArray.msg
		MonitorInfo.msg
		CameraCalibrationCmd.msg
		CameraQrTrackState.msg
		LiftRunCmd.msg
		PassagewayCmd.msg
		PassageDeviceCmd.msg
		Task.msg
		OrbbecCameraSwitch.msg
		
		MapManagerCmd.msg
		SpeedLimit.msg
		SmootherVariable.msg
		
		BaseNormalBits.msg
		BaseErrorBits.msg
		BaseBits.msg
		BaseDrvCanError.msg
		BaseBatteryState.msg
		BaseOutSig.msg
		BaseRemote.msg
		BaseState2.msg
		BasePidState.msg
		HealthState.msg
		BaseUpgradeState.msg
		TFmini.msg
		TFminiArray.msg
		CanPower.msg
		CanDriver.msg 
		CanState.msg
		VehicleState.msg

		# PlatformAuth.msg
		MappingData.msg
		ReportWarning.msg

		HubMapInfo.msg
		MapDownloadResult.msg

		SpeedRegulator.msg
		SpeedRegulatorArray.msg

		WifiOperationCmd.msg
		CrossLevelPathInfo.msg
		CrossLevelInfo.msg

		ZoneCleanPath.msg
		ZoneCleanPathArray.msg

		PathInfo.msg
		PathArray.msg
		Zone.msg
		ZoneArray.msg
		MoveBaseState.msg
		AutoTest.msg

		LoraCmd.msg
		LoraMsg.msg

		SensorSwitch.msg

		BoxManagerRequest.msg
		BoxManagerResponse.msg
		
		PoseWithWeight.msg
		PoseArrayWeighted.msg

		SelfLocationRequest.msg
		SelfLocationResponse.msg

		NearbyRobotInfo.msg
		MultiNearbyRobot.msg

		OneWashBoxRequest.msg

		BackgardenRobotInfo.msg
		BackgardenElevatorInfo.msg
		BackgardenBoxInfo.msg
		BackgardenDownloadInfo.msg

		Graph.msg
		Vertex.msg

		TOFSwitch.msg
		RealsenseSwitch.msg

		SegmentLine.msg

		ComputerBaseInfo.msg

		MQTTFlag.msg

		Goal.msg

		ElevatorCloudRequest.msg
)

add_service_files(
	FILES
		# BoxOperationService.srv
		BoxBatteryQueryByCloud.srv
		
		GetVisionPoseService.srv
		GetZoneMapService.srv
		GetZonesService.srv
		GetRingIslandService.srv
		GetAllPaths.srv

		### map_manager provide services ###
		# GetMapSetService.srv
		IsFloorExistService.srv
		GetCurrentMapsInUseService.srv
		GetGatePoseService.srv
		GetGateInfoBySSIDService.srv
		GetElevatorInfoForNaviService.srv
		GetCrossLevelPathService.srv
		GetAllChargingStationService.srv
		GetDockInfoService.srv
		GetLoraFrequencyService.srv
		GetAllWayPoints.srv
		GetMapInfoByBuildingAndFloor.srv
		CheckTwoFloorSame.srv
		GetBuildingDetailedInfo.srv

		GetCloudPaths.srv
		GetCloudZones.srv

		### multi_map_server provide services ###
		ChangeMapAndLocationServer.srv

		### robot_dock provide services ###
		DoCanStopMounting.srv

		### state machine task service ###
		CloudTaskService.srv
		CanTaskStopService.srv
		GetCurrentRobotInfo.srv
		
		### navigation control and auto config ###
		DynamicParamView.srv

		CtiCommonService.srv

		### end--

		### map segment service ###
		GetMapSegmentService.srv
		### sound manager ###
		VolumeOperation.srv

		OperationNetwork.srv

		### dispath interface ###
		DispathRequest.srv
)

add_action_files(
	FILES
		GlobalPlanner.action
		MapsDownloadRequest.action
		QueryHubMapsInfo.action
		QueryCloudDockInfo.action
		SecurityPassageWayManager.action
		NavigationDecider.action
		KnockDoor.action
)

generate_messages(
	DEPENDENCIES
		actionlib_msgs
		geometry_msgs
		sensor_msgs
		nav_msgs
		std_msgs
)

catkin_package(
#  INCLUDE_DIRS include
#  LIBRARIES cti_msg
	CATKIN_DEPENDS message_runtime actionlib_msgs geometry_msgs sensor_msgs nav_msgs std_msgs
#  DEPENDS system_lib
)

include_directories(
#  proto/
	${catkin_INCLUDE_DIRS}
)

