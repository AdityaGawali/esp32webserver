#include <stdio.h>
#include <string.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"
#include "esp_wifi.h"
#include "esp_event_loop.h"
#include "nvs_flash.h"
#include "esp_log.h"

#include "driver/gpio.h"
#include "mdns.h"

#include "lwip/api.h"
#include "lwip/err.h"
#include "lwip/netdb.h"


// HTTP headers and web pages
const static char http_html_hdr[] = "HTTP/1.1 200 OK\nContent-type: text/html\n\n";
const static char http_off_hml[] = "<meta content=\"width=device-width,initial-scale=1\"name=viewport><style>div{width:230px;height:300px;position:absolute;top:0;bottom:0;left:0;right:0;margin:auto}</style><div><h1 align=center>Relay is OFF</h1><a href=on.html><img src=on.png></a></div>";
const static char http_on_hml[] = "<meta content=\"width=device-width,initial-scale=1\"name=viewport><style>div{width:230px;height:300px;position:absolute;top:0;bottom:0;left:0;right:0;margin:auto}</style><div><h1 align=center>Relay is ON</h1><a href=off.html><img src=off.png></a></div>"; 

////////////
//const static char aditya_html[] = "<h1>Aditya is here</h1>\n<p>Aditya made this</p>\n<h2><a href=bobby.html>bobby</a></h2>";
//const static char bobby_html[] = "<h1>bobby is here</h1>\n<p>bobby made this</p>\n<h2><a href=aditya.html>aditya</a></h2>";
const static char default_html[] = "<h1><a href =aditya.html>Increase</h1>\n<h1><a href =bobby.html>decrease</h1>";

//extern const uint8_t aditya_html_start[] asm("_binary_aditya_html_start");
//extern const uint8_t aditya_html_end[]   asm("_binary_aditya_html_end");
///////////

char countarr[100];
char countString[]="Count : ";
float count=0.0;

// Event group for inter-task communication
static EventGroupHandle_t event_group;
const int WIFI_CONNECTED_BIT = BIT0;


static esp_err_t event_handler(void *ctx, system_event_t *event) // Wifi event handler
{
    switch(event->event_id) {
		
    case SYSTEM_EVENT_STA_START:
        esp_wifi_connect();
        break;
    
	case SYSTEM_EVENT_STA_GOT_IP:
        xEventGroupSetBits(event_group, WIFI_CONNECTED_BIT);
        break;
    
	case SYSTEM_EVENT_STA_DISCONNECTED:
		xEventGroupClearBits(event_group, WIFI_CONNECTED_BIT);
        break;
    
	default:
        break;
    }  
	return ESP_OK;
}

static void http_server_netconn_serve(struct netconn *conn) //Serve the Webpage and extract Requests  
{


	struct netbuf *inbuf; //Incoming buffer
	char *buf;
	u16_t buflen;
	err_t err;
	err = netconn_recv(conn, &inbuf);

	if (err == ERR_OK) 
	{
	  
		netbuf_data(inbuf, (void**)&buf, &buflen);
		// extract the first line, with the request
		char *first_line = strtok(buf, "\n");
		if(first_line) 
		{
			
			// default page
			if(strstr(first_line, "GET / ")) 
			{
				netconn_write(conn, http_html_hdr, sizeof(http_html_hdr) - 1, NETCONN_NOCOPY);
				netconn_write(conn, default_html, sizeof(default_html) - 1, NETCONN_NOCOPY);
				count =0.0;
				//netconn_write(conn, aditya_html_start, aditya_html_end-aditya_html_start, NETCONN_NOCOPY);

				printf("Default was requested \n");									

			}
			else if(strstr(first_line, "GET /aditya.html ")) 
			{
				//netconn_write(conn, http_html_hdr, sizeof(http_html_hdr) - 1, NETCONN_NOCOPY);
				//netconn_write(conn, aditya_html, sizeof(aditya_html) - 1, NETCONN_NOCOPY);
				//printf("Aditya was requested \n");

				netconn_write(conn, http_html_hdr, sizeof(http_html_hdr) - 1, NETCONN_NOCOPY);
				netconn_write(conn, default_html, sizeof(default_html) - 1, NETCONN_NOCOPY);
				for(int i = 0 ; i<100;i++)
				{
				count += 1; 
				gcvt(count,4,countarr);
				netconn_write(conn, countString, sizeof(countString) - 1, NETCONN_NOCOPY);
				netconn_write(conn, countarr, sizeof(countarr) - 1, NETCONN_NOCOPY);
				vTaskDelay(100);
				}			
			}
			else if(strstr(first_line, "GET /bobby.html ")) 
			{
				//netconn_write(conn, http_html_hdr, sizeof(http_html_hdr) - 1, NETCONN_NOCOPY);
				//netconn_write(conn, bobby_html, sizeof(bobby_html) - 1, NETCONN_NOCOPY);
				//printf("bobby was requested \n");

				netconn_write(conn, http_html_hdr, sizeof(http_html_hdr) - 1, NETCONN_NOCOPY);
				netconn_write(conn, default_html, sizeof(default_html) - 1, NETCONN_NOCOPY);
				count -= 1;
				gcvt(count,4,countarr);
				netconn_write(conn, countString, sizeof(countString) - 1, NETCONN_NOCOPY);
				netconn_write(conn, countarr, sizeof(countarr) - 1, NETCONN_NOCOPY);
			}
			

			else printf("Unkown request: %s\n", first_line);
		}
		else printf("Unkown request\n");
	}
	
	// close the connection and free the buffer
	netconn_close(conn);
	netbuf_delete(inbuf);
}

static void http_server(void *pvParameters) //Binds netconn and listens
{
	
	struct netconn *conn, *newconn;
	err_t err;
	conn = netconn_new(NETCONN_TCP);
	netconn_bind(conn, NULL, 80);
	netconn_listen(conn);
	printf("HTTP Server listening...\n");
	do {
		err = netconn_accept(conn, &newconn);
		printf("New client connected\n");
		if (err == ERR_OK) {
			http_server_netconn_serve(newconn);
			netconn_delete(newconn);
		}
		vTaskDelay(1); //allows task to be pre-empted
	} while(err == ERR_OK);
	netconn_close(conn);
	netconn_delete(conn);
	printf("\n");
}


void wifi_setup() //Sets up the main  Wifi Driver 
{
	
	event_group = xEventGroupCreate();
	
	tcpip_adapter_init();
	
	ESP_ERROR_CHECK(esp_event_loop_init(event_handler, NULL));

	wifi_init_config_t wifi_init_config = WIFI_INIT_CONFIG_DEFAULT();
	
	ESP_ERROR_CHECK(esp_wifi_init(&wifi_init_config));
	ESP_ERROR_CHECK(esp_wifi_set_storage(WIFI_STORAGE_RAM));
	ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA));

	wifi_config_t wifi_config ={
        .sta = {
            .ssid = CONFIG_WIFI_SSID,
            .password = CONFIG_WIFI_PASSWORD,
        },
    };

	ESP_ERROR_CHECK(esp_wifi_set_config(ESP_IF_WIFI_STA, &wifi_config));
    ESP_ERROR_CHECK(esp_wifi_start());
}









void app_main()
{
	esp_log_level_set("wifi", ESP_LOG_NONE); //No wifi logging

	nvs_flash_init(); //some random init ,I dont know

	wifi_setup(); // wifi driver setup

	// wait for connection
	printf("Waiting for connection to the wifi network...\n ");
	xEventGroupWaitBits(event_group, WIFI_CONNECTED_BIT, pdFALSE, pdTRUE, portMAX_DELAY);
	printf("Connected\n\n");

	//Print local IP address
	tcpip_adapter_ip_info_t ip_info;
	ESP_ERROR_CHECK(tcpip_adapter_get_ip_info(TCPIP_ADAPTER_IF_STA, &ip_info));
	printf("IP Address:  %s\n", ip4addr_ntoa(&ip_info.ip));
	printf("Subnet mask: %s\n", ip4addr_ntoa(&ip_info.netmask));
	printf("Gateway:     %s\n", ip4addr_ntoa(&ip_info.gw));	

	//Start http Server Task
	xTaskCreate(&http_server, "http_server", 2048, NULL, 5, NULL);

}