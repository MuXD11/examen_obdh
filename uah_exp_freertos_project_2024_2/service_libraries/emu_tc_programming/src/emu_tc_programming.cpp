#include <public/emu_hw_timecode_drv_v1.h>
#include <public/emu_sc_channel_drv_v1.h>
#include <public/emu_gss_v1.h>


#define FT_UAH_EXP_SERV_129_0010



#ifdef  FT_UAH_EXP_SERV_129_0010

#define FT_UAH_EXP_SERV_129_0010_step0 (UNITIME_AFTER_POWER_ON + 10)
#define FT_UAH_EXP_SERV_129_0010_step1 (UNITIME_AFTER_POWER_ON + 20)
#define FT_UAH_EXP_SERV_129_0010_step2 (UNITIME_AFTER_POWER_ON + 40)
#define FT_UAH_EXP_SERV_129_0010_step3 (UNITIME_AFTER_POWER_ON + 60)
#define FT_UAH_EXP_SERV_129_0010_step4 (UNITIME_AFTER_POWER_ON + 70)

//configurar constantes de control
EmuGSS_TCProgram129_2 prog_FT_0010_step_0(FT_UAH_EXP_SERV_129_0010_step0,
                "FT_UAH_EXP_SERV_129_0080 step 0, Set Kp", 0.15, 0.45);


//velocidades objetivo
EmuGSS_TCProgram129_1 prog_FT_0010_step_1( FT_UAH_EXP_SERV_129_0010_step1,
                    "FT_UAH_EXP_SERV_129_0080 step 1, Set Vx and Vy ", 0.5 ,-0.5 );


//finalizar el test
EmuGSS_TCProgram128_1 prog_FT_0010_step_4(FT_UAH_EXP_SERV_129_0010_step4,
                    "FT_UAH_EXP_SERV_129_0080  001 step 4, Fin del test ");
#endif

