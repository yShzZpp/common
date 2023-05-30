## divide paramters into three level:
###基本参数(所有机器都一样<上库>), 机器版本相关参数(同一版本机器都一样<上库>), 机器号相关参数(每台机器都不一样<保存在机器本地>)
params_base:          parameters support all machine type and machine id
params_machine_type:  parameters support specific machine type
params_machine_id:    parameters only support current machine

## attention
current folder is params_machine_type 

## rules for file name
${node_name}_params.yaml


