#! /bin/bash

ntp_flag=0
try_times=600

while [ ${ntp_flag} == 0 ]
do
    sleep 1
    ntp_state=`timedatectl | grep "NTP synchronized" | cut -d ' ' -f 3`
    echo "ntp_state: "${ntp_state}
    if [[ ${ntp_state} == yes ]]; then
        echo "ntp success"
        ntp_flag=1
    else
        let "try_times--"
        echo "still try ${try_times} times"
        if [[ ${try_times} == 0 ]]; then
            echo "reboot computer after 30s"
            sleep 30
            reboot
        fi
    fi
done

