#! /bin/bash
#author: yxl

robot_home=/home/cti

## some robot running mode file #####
navimode_file=${robot_home}/.ros/flag_naviation
mapmode_file=${robot_home}/.ros/flag_mapping
allmode_file=${robot_home}/.ros/flag_*
mapping_file=${robot_home}/.ros/flag_mapping_run
reboot_file=${robot_home}/.ros/flag_reboot

## upgrade info file #####
upgrade_file_current=${robot_home}/.ros/upgrade/upgrade_info_current
upgrade_file=${robot_home}/.ros/upgrade/upgrade_info
upgrade_motion_file=${robot_home}/.ros/upgrade/upgrade_motion_info
rollback_file=${robot_home}/.ros/upgrade/rollback_fail
robotinfo_file=${robot_home}/.ros/robot_config/robot_info
motion_upgrade_completed_file=${robot_home}/.ros/upgrade/motion_upgrade_completed

## scripts dir #####
scripts_dir="/opt/cti/kinetic/share/br_bringup/scripts"

## package name #####
package_name="br-robot-release"

## report result shell #####
report_result_sh=${robot_home}/.ros/upgrade/upload_ota_state

start_br() {
	echo "start br navi mode. come on ピカチュウ"
	export HOME=${robot_home}

	#增加配置网卡的过程 先屏蔽掉,尚未适配旧版机器
	#echo "set up network..."
	#su root -c "${scripts_dir}/setup_network.sh"

	echo "check ntp..."
	su root -c "${scripts_dir}/check_ntp.sh"

	echo "start br nodes..."
	su cti -c "${scripts_dir}/start_br.sh"
	sleep 1
}

start_map() {
    echo "start br mapping mode. come on ヒトカゲ"
	export HOME=${robot_home}
	su cti -c "${scripts_dir}/start_map.sh"
    sleep 1
}

start_mapping() {
	echo "start br test mapping node. come on フシギダネ"
	export HOME=${robot_home}
	su cti -c "${scripts_dir}/start_karto.sh"
    sleep 1
}

start_motion_upgrade() {
	echo "start upgrade embedded system."
	export HOME=${robot_home}
	su cti -c "${scripts_dir}/start_motion_upgrade.sh"
    sleep 1
}

stop_ros() {
	echo "kill all ros nodes. so cruel ..."
	killall roslaunch
	sleep 60
}

robot_reboot() {
	echo "reboot computer..."
	sleep 1
	reboot
}

change_attribute() {
	echo "start to change robot attribute"
	while read line
	do
		if [[ "$line" = robot_model* ]]; then
			local current_model=$(echo $line | cut -d " " -f 2)
			local new_mode=$(echo $line | cut -d " " -f 3)
			echo "robot model changed, from "${current_model}" to "${new_mode}
			sed -i "4s/value=\"${current_model}\"/value=\"${new_mode}\"/g" /opt/cti/kinetic/share/br_bringup/launch/start_br.launch
			sed -i "4s/value=\"${current_model}\"/value=\"${new_mode}\"/g" /opt/cti/kinetic/share/br_bringup/launch/start_map.launch
		elif [[ "$line" = robot_num* ]]; then
			local current_num=$(echo $line | cut -d " " -f 2)
			local new_num=$(echo $line | cut -d " " -f 3)
			echo "robot model changed, from "${current_num}" to "${new_num}
			sed -i "5s/value=\"${current_num}\"/value=\"${new_num}\"/g" /opt/cti/kinetic/share/br_bringup/launch/start_br.launch
			sed -i "5s/value=\"${current_num}\"/value=\"${new_num}\"/g" /opt/cti/kinetic/share/br_bringup/launch/start_map.launch
		elif [[ "$line" = robot_function* ]]; then
			local current_function=$(echo $line | cut -d " " -f 2)
			local new_function=$(echo $line | cut -d " " -f 3)
			echo "robot model changed, from "${current_function}" to "${new_function}
			sed -i "6s/value=\"${current_function}\"/value=\"${new_function}\"/g" /opt/cti/kinetic/share/br_bringup/launch/start_br.launch
		fi
	done < $robotinfo_file
	echo "change attribute end --"
}

get_package_version() {
	local lo_package_name=$1
	version=`dpkg -s ${lo_package_name} | grep Version`
    echo "${version##* }"
}

get_package_state() {
	local lo_package_name=$1
	status=`dpkg -s ${lo_package_name} | grep Status: `
	echo "${status#* }"
}

get_package_md5() {
	local lo_package=$1
	sum=`md5sum ${lo_package}`
	echo "${sum%% *}"
}

report_update_result()
{
	if [[ -f ${report_result_sh} ]]; then
		. ${report_result_sh} $1 $2
	else
		echo "${report_result_sh} isn't exist..."
	fi
}

check_upgrade_success() {
	local lo_param_package_version=$1
	local lo_param_package_name=$2

	local lo_check_version=$(get_package_version ${lo_param_package_name})
	if [[ "$lo_param_package_version" != "$lo_check_version" ]]; then
		echo "upgrade fail, package version not same : ${lo_param_package_version}:::${lo_check_version}"
		report_update_result 2 "导航deb升级失败:升级后的版本与配置文件中的版本不一致.${lo_param_package_version}:${lo_check_version}"
		return 1
	fi

	local lo_check_state=$(get_package_state ${lo_param_package_name})
	if [[ "${lo_check_state}" != "install ok installed" ]]; then
		echo "upgrade fail, package state error: ${lo_check_state}"
		report_update_result 2 "导航deb升级失败:版本在安装过程中异常:[${lo_check_state}]"
		return 2
	fi
	return 0
}

install_deb() {
	local lo_param_package_name=$1
	local lo_param_package_file=$2

	echo "install deb ${lo_param_package_file}"
	stop_ros
	dpkg -r ${lo_param_package_name}
	sleep 10
	dpkg -i ${lo_param_package_file}
	sleep 2
}

# get_config_info() {
# 	local lo_param_config_file=$1

# 	echo "get config info : $lo_param_config_file"
# 	while read line
# 	do
# 		if [[ "$line" = upgrade_imme* ]]; then
# 			eval $2=${line#*:}
# 		elif [[ "$line" = package:* ]]; then
# 			eval $3=${line#*:}
# 		elif [[ "$line" = package_version:* ]]; then
# 			eval $4=${line#*:}
# 		elif [[ "$line" = package_md5:* ]]; then
# 			eval $5=${line#*:}
# 		fi
# 	done < $lo_param_config_file
# }

check_package() {
	local lo_param_upgrade_package_file=$1
	local lo_param_upgrade_package_version=$2
	local lo_param_upgrade_package_md5=$3
	local lo_param_upgrade_package_name=$4

	echo "check package : ${lo_param_upgrade_package_file}"
	report_update_result 2 "检查deb文件"
	
	if [[ "${lo_param_upgrade_package_file}" != ${robot_home}/.ros/upgrade/${lo_param_upgrade_package_name}_V*.deb ]]; then
		echo "check fail: package name error : ${lo_param_upgrade_package_file}"
		report_update_result 2 "升级包名字异常:${lo_param_upgrade_package_file}"
		return 1
	else
		echo "check pass: package name ok"
	fi

	if [[ ! -f ${lo_param_upgrade_package_file} ]]; then
		echo "check fail: package isn't exist : ${lo_param_upgrade_package_file}"
		report_update_result 2 "deb文件不存在:${lo_param_upgrade_package_file}"
		return 2
	else
		echo "check pass: package exist"
	fi

	local lo_package_md5=$(get_package_md5 ${lo_param_upgrade_package_file})
	if [[ "${lo_param_upgrade_package_md5}" != "${lo_package_md5}" ]]; then
		echo "check fail: md5 error: ${lo_param_upgrade_package_md5} ${lo_package_md5}"
		report_update_result 2 "deb文件md5校验不过:${lo_param_upgrade_package_file}"
		return 3
	else
		echo "check pass: package md5 same"
	fi

	local lo_package_version=$(get_package_version ${lo_param_upgrade_package_name})
	if [[ "${lo_param_upgrade_package_version}" == "${lo_package_version}" ]]; then
		echo "robot has same version package ${lo_param_upgrade_package_version}"
		local lo_package_state=$(get_package_state ${lo_param_upgrade_package_name})
		# if [[ "${lo_package_state}" != "deinstall ok config-files" ]]; then
		if [[ "${lo_package_state}" == "install ok installed" ]]; then
			echo "check fail: the ${lo_param_upgrade_package_version} package has installed, don't need upgrade"
			report_update_result 2 "deb版本已安装"
			return 4
		else
			echo "check pass: package not installed: ${lo_package_state}"
		fi
	else
		echo "check pass: version not same. new:${lo_param_upgrade_package_version} old:${lo_package_version}"
	fi

	report_update_result 2 "检查deb文件完成"
	return 0
}	

# param 1: package name
# param 2: upgrade file
# param 3: current upgrade file
rollback() {
	local lo_param_package_name=$1
	local lo_param_new_upgrade_config_file=$2
	local lo_param_current_upgrade_config_file=$3

	echo "start to rollback to old version"
	report_update_result 2 "版本回滚开始"

	local lo_old_upgrade_moment=0
	local lo_old_upgrade_package=""
	local lo_old_upgrade_package_version=""
	local lo_old_upgrade_package_md5=""

	while read line
	do
		if [[ "$line" = upgrade_imme* ]]; then
			lo_old_upgrade_moment=${line#*:}
		elif [[ "$line" = package:* ]]; then
			lo_old_upgrade_package=${line#*:}
		elif [[ "$line" = package_version:* ]]; then
			lo_old_upgrade_package_version=${line#*:}
		elif [[ "$line" = package_md5:* ]]; then
			lo_old_upgrade_package_md5=${line#*:}
		fi
	done < ${lo_param_current_upgrade_config_file}
	
	echo "old upgrade config moment="${lo_old_upgrade_moment}
	echo "old upgrade config package="${lo_old_upgrade_package}
	echo "old upgrade config version="${lo_old_upgrade_package_version}
	echo "old upgrade config md5="${lo_old_upgrade_package_md5}

	local ret=0
	check_package ${lo_old_upgrade_package} \
				  ${lo_old_upgrade_package_version} \
				  ${lo_old_upgrade_package_md5} \
				  ${lo_param_package_name}
        ret=$?
	if [[ ${ret} != 0 ]]; then
		echo "check old package fail, error code: ${ret}"
		rm ${lo_param_new_upgrade_config_file}
		report_update_result 2 "版本回滚时:校验deb文件失败"
		return 1
	fi

	install_deb ${lo_param_package_name} ${lo_old_upgrade_package} 

	check_upgrade_success ${lo_old_upgrade_package_version} ${lo_param_package_name}
	ret=$?
	if [[ ${ret} != 0 ]]; then
		echo "check old package installed fail, error code: ${ret}"
		report_update_result 2 "版本回滚时:deb安装失败"
		return 2
	fi

	report_update_result 2 "版本回滚成功"
	return 0
}

#
# param 1: upgrade time
# param 2: package name
# param 3: new upgrade file
# param 4: current upgrade file
upgrade_deb() {
	local lo_param_upgrade_imme=$1
	local lo_param_package_name=$2
	local lo_param_new_upgrade_config_file=$3
	local lo_param_current_upgrade_config_file=$4

	echo "start to upgrade"
	report_update_result 2 "导航升级中"

	local lo_new_upgrade_moment=0
	local lo_new_upgrade_package=""
	local lo_new_upgrade_package_version=""
	local lo_new_upgrade_package_md5=""

	while read line
	do
		if [[ "$line" = upgrade_imme* ]]; then
			lo_new_upgrade_moment=${line#*:}
		elif [[ "$line" = package:* ]]; then
			lo_new_upgrade_package=${line#*:}
		elif [[ "$line" = package_version:* ]]; then
			lo_new_upgrade_package_version=${line#*:}
		elif [[ "$line" = package_md5:* ]]; then
			lo_new_upgrade_package_md5=${line#*:}
		fi
	done < ${lo_param_new_upgrade_config_file}

	echo "new upgrade config moment="${lo_new_upgrade_moment}
	echo "new upgrade config package="${lo_new_upgrade_package}
	echo "new upgrade config version="${lo_new_upgrade_package_version}
	echo "new upgrade config md5="${lo_new_upgrade_package_md5}

	if [[ ${lo_param_upgrade_imme} != ${lo_new_upgrade_moment} ]]; then
		echo "update moment error: ${lo_param_upgrade_imme}, ${lo_new_upgrade_moment}"
		report_update_result 1 "导航升级失败,非升级时间"
		return 1
	fi

	local ret=0
	check_package ${lo_new_upgrade_package} \
		      ${lo_new_upgrade_package_version} \
		      ${lo_new_upgrade_package_md5} \
		      ${lo_param_package_name}
	ret=$?
	if [[ ${ret} != 0 ]]; then
	  	echo "check package fail, error code: ${ret}"
		rm ${lo_param_new_upgrade_config_file}
		report_update_result 1 "导航升级失败,deb校验失败"
		return 2
	fi

	install_deb ${lo_param_package_name} ${lo_new_upgrade_package} 
	check_upgrade_success ${lo_new_upgrade_package_version} ${lo_param_package_name}
	ret=$?
	if [[ ${ret} != 0 ]]; then
		echo "check new package installed fail, error code: ${ret}. then rollback current version"
		report_update_result 2 "导航deb升级失败,回滚到旧版本"
		rollback ${lo_param_package_name} ${lo_param_new_upgrade_config_file} ${lo_param_current_upgrade_config_file}
		ret=$?
		if [[ ${ret} != 0 ]]; then
			echo "I don't know what happened. Maybe a meteorite fell and broke the machine."
			rm ${lo_param_new_upgrade_config_file}
			rm ${lo_param_current_upgrade_config_file}
			touch ${rollback_file}
			report_update_result 1 "回滚版本失败,机器可能被陨石砸到了"                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                 }
			return 3
		else
			echo "rollback old package success"
			rm ${lo_param_new_upgrade_config_file}
			report_update_result 0 "回滚版本成功,机器恢复到旧版本"
			start_br
			return 4
		fi
	else
		echo "upgrade new package success"
		mv ${lo_param_new_upgrade_config_file} ${lo_param_current_upgrade_config_file}
		report_update_result 0 "导航deb升级成功"
		start_br
	fi

	return 0
}

update_motion_control() {
	report_update_result 2 "运控升级中"

	#删除旧的升级完成标志文件
	echo "delete old motion upgrade completed file"
	rm ${motion_upgrade_completed_file}

	sleep 10
	stop_ros
	sleep 10

	start_motion_upgrade
	while [ ! -f ${motion_upgrade_completed_file} ]
	do
		sleep 2
	done

	local ret=`cat ${motion_upgrade_completed_file}`
	echo "motion control update result:${ret}"
	rm ${motion_upgrade_completed_file}
	if [ ${ret} -eq 0 ]; then
		report_update_result 0 "运控升级成功"
	elif [ ${ret} -eq 2 ]; then
		report_update_result 1 "运控版本校验失败"
	else
		report_update_result 1 "运控升级失败"
	fi

	rm ${upgrade_motion_file}

	sleep 10
	stop_ros
	sleep 10
	start_br
	return ${ret}
}

######## start ##############################################################################

echo "start Br Manager--check files--"

rm ${allmode_file}

if [ -f ${mapping_file} ]; then 
	rm ${mapping_file} 
fi

#先判断是否是重启升级
sleep 1
echo "start Br Manager--check upgrade--"
if [ -f ${upgrade_file} ]; then
	upgrade_deb 0 ${package_name} ${upgrade_file} ${upgrade_file_current}
fi

start_br

echo "wait 20s for nodes running--"
sleep 20

while true
do
	sleep 10
	if [ -f ${navimode_file} ]; then
		echo "--oh-- we need change to navi mode"
		stop_ros
		start_br
		rm ${allmode_file}
		echo "navi mode running..."

	elif [ -f ${mapmode_file} ]; then
		echo "--oh-- we need change to mapping mode"
		stop_ros
		start_map
		rm ${allmode_file}
        echo "mapping mode running...good luck, have fun..."

	elif [ -f ${mapping_file} ]; then
		echo "--oh-- we need run mapping node"
		start_mapping
        rm ${mapping_file}
        echo "mapping running...good luck, have fun..."

	elif [ -f ${upgrade_file} ]; then
		echo "--oh-- we need check upgrade file"
		sleep 1 #wait, write file
		upgrade_deb 1 ${package_name} ${upgrade_file} ${upgrade_file_current}

	elif [ -f ${upgrade_motion_file} ]; then
		echo "--oh-- we need upgrade motion file"
		sleep 1 #wait, write file
		update_motion_control
		
	elif [ -f ${robotinfo_file} ]; then
		echo "--hhh-- we need change robot basic attribute"
		stop_ros
		change_attribute
		rm ${robotinfo_file}
		start_br
		echo "--hhh-- robot basic attribute has changed"
	
	elif [ -f ${reboot_file} ]; then
		echo "--oh-- we need upgrade motion file"
		rm ${reboot_file}
		robot_reboot

	else
		echo "nothing need to do"
		# su root -c "${scripts_dir}/check_ping.sh"
	fi
done

exit 0
