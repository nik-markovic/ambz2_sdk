#include <platform/platform_stdlib.h>
#include <platform_opts.h>
#include <platform_opts_bt.h>
#include "FreeRTOS.h"
#include "task.h"
#include "bt_config_app_task.h"
#include "wifi_constants.h"
#include "wifi_conf.h"
#include "bt_config_app_main.h"

#if CONFIG_BT

#if defined(CONFIG_PLATFORM_8710C) && defined(CONFIG_FTL_ENABLED)
#include <ftl_int.h>

const uint8_t ftl_phy_page_num = 3;	/* The number of physical map pages, default is 3: BT_FTL_BKUP_ADDR, BT_FTL_PHY_ADDR1, BT_FTL_PHY_ADDR0 */
const uint32_t ftl_phy_page_start_addr = BT_FTL_BKUP_ADDR;

extern void fATBB(void *);
static void bt_auto_init_task(void *param)
{
    (void) param;
    printf("bt_auto_init_task: Starting...\n\r");
	/*Check WIFI init complete*/
	while( ! (wifi_is_up(RTW_STA_INTERFACE) || wifi_is_up(RTW_AP_INTERFACE))) {
		printf("bt_auto_init_task: WIFI is not ready. Waiting...\n\r");
          vTaskDelay(500 / portTICK_PERIOD_MS);
	}

	fATBB("1");
	vTaskDelete(NULL);

}
#define STACKSIZE                   (4 * 1024)
void app_ftl_init(void)
{
	ftl_init(ftl_phy_page_start_addr, ftl_phy_page_num);
	if(xTaskCreate(bt_auto_init_task, ((const char*)"BT Start"), STACKSIZE, NULL, tskIDLE_PRIORITY + 3 + PRIORITIE_OFFSET, NULL) != pdPASS)
		printf("\n\r%s xTaskCreate(bt_auto_init_task) failed", __FUNCTION__);
}
#endif //CONFIG_PLATFORM_8710C && CONFIG_FTL_ENABLED

#endif //CONFIG_BT
