我的完工文件目录：
Core >
      Inc>
           arm_cfg.h: 这个文件主要规定了 角度、舵机关节和时基 等等基础的数据：任务书 T1 要求“映射参数写在配置文件或宏里，不散落在代码中”
           arm_joint.h:这个文件其实是一个调度文件，很重要。核心价值就是：接口统一，实现分离
           arm_pwm.h：这是一个纯粹的计算模块，在接收到joint分发下来的数据之后呢，直接把角度（deg）->脉宽（us）->计数值（CCR）算进寄存器
           arm_pid.h:pid算法服务于360底座的闭环，这个没法简单说，建议找视频问ai，这里只能提到大概和补充细节
           as5600.h
           mpu6050.h
           kin_scs.h
           kin_ccs.h
           kin_c2s.h
           proto.h
           proto_app.h
           proto_link.h
           main.h
           stm32f1xx_hal_conf.h
           stm32f1xx_it.h
    Src >
           arm_cfg.c
           arm_joint.c
    arm_pwm.c：具体怎么算呢：收到joint发来的关节角度->把舵机不认识的角度换成认识的PWM信号，即高电平长度（单位us）->再把这个us换成计数值（CCR）->写进寄存器
               对应的宏定义和函数名称：arm_set_deg(joint, deg) -> arm_deg_to_us(deg) -> ARM_PWM_US_TO_CCR（us）-> arm_write_ccr(joint, ccr) -> __HAL_TIM_SET_COMPARE(&htim2, s_channel[joint], ccr)
                            ## arm_set_deg(joint, deg):joint说明是哪个关节，deg说明上位机传进的角度
                            ## arm_write_ccr(joint, ccr)：这是个中间函数，暂时不懂很正常，这个要放在调用关系里看
                            ## ARM_PWM_US_TO_CCR（us）：自定义宏，把us换成计数值CCR
                            ## arm_write_ccr(joint, ccr）：其实就是把__HAL_TIM_SET_COMPARE封了一层，方便改东西
                                ----------------------------------------下面的调用关系非常重要，我打赌答辩百分百会问到----------------------------------------
                                先看两个函数实现：

                                 void arm_set_deg(uint8_t joint, float deg)//本质就是把目标关节和目标角度信息打包发过来
                                 {
                                       if (joint >= ARM_JOINT_NUM)//防止越界
                                       {
                                            return;
                                        }
                                    arm_write_ccr(joint, arm_deg_to_ccr(joint, deg));
                                  }

                                 uint16_t arm_deg_to_ccr(uint8_t joint, float deg)
                                 {
                                      return ARM_PWM_US_TO_CCR(arm_deg_to_us(joint, deg));
                                  }

           arm_pid.c
           as5600.c
           mpu6050.c
           kin_scs.c
           kin_ccs.c
           kin_c2s.c
           proto.c
           proto_app.c
           proto_link.c
           main.c
           stm32f1xx_hal_msp.c
           stm32f1xx_it.c