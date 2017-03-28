#include <string.h>
#include <stdio.h>
#include <time.h>
#include "contiki.h"
#include "contiki-net.h"
#include "awa/static.h"

#ifdef CONTIKI_TARGET_MINIMAL_NET
#include <arpa/inet.h>
#endif

// minimal-net didn't seem to include this by default
#ifdef PROJECT_CONF_H
#include PROJECT_CONF_H
#endif /* PROJECT_CONF_H */

#define FREQ_OBJID          27000
#define FREQ_VALUE          5700
#define FREQ_UNITS          5701
#define FREQ_MEASURED_MIN   5601
#define FREQ_MEASURED_MAX   5602
#define FREQ_RANGE_MIN      5603
#define FREQ_RANGE_MAX      5604
#define FREQ_RESET          5605
#define FREQ_CALIBRATION    5821
#define FREQ_APP_TYPE       5750

#define FREQ_INSTANCES      1

typedef struct
{
    AwaFloat value;
    AwaFloat units;
    AwaFloat measured_max;
    AwaFloat measured_min;
    AwaFloat range_max;
    AwaFloat range_min;
    char calibration[64];
    char app_type[64];

} FrequencyObject;

static FrequencyObject freq_obj[FREQ_INSTANCES];


AwaResult handler(AwaStaticClient * client, AwaOperation operation, AwaObjectID objectID,
                  AwaObjectInstanceID objectInstanceID, AwaResourceID resourceID, AwaResourceInstanceID resourceInstanceID,
                  void ** dataPointer, size_t * dataSize, bool * changed)
{
    AwaResult result = AwaResult_InternalError;
    printf("handler called. operation = %d \n", operation);
    if ((objectID == FREQ_OBJID) && (objectInstanceID >= 0) && (objectInstanceID < FREQ_INSTANCES))
    {
        switch (operation)
        {
            case AwaOperation_CreateObjectInstance:
            case AwaOperation_CreateResource:
            {
                result = AwaResult_SuccessCreated;
                break;
            }
            case AwaOperation_Read:
            case AwaOperation_Write:
            {
                printf("operation = %d \n", operation);

                result = AwaResult_SuccessContent;
                break;
            }
            case AwaOperation_Execute:
            {
                switch (resourceID)
                {
                case FREQ_RESET:
                    printf("RESET CALLED!!!\n");
                    freq_obj[objectInstanceID].measured_max = 0.0;
                    freq_obj[objectInstanceID].measured_min = 0.0;
                    AwaStaticClient_ResourceChanged(client, FREQ_OBJID, 0, FREQ_MEASURED_MAX);

                    break;
                default:
                    break;
                }
                result = AwaResult_Success;
                break;
            }
            default:
                break;
        }
    }
    return result;
}

static void DefineFrequencyObject(AwaStaticClient * awaClient)
{
    int instance;

    AwaStaticClient_DefineObject(awaClient, FREQ_OBJID, "Frequency", 0, FREQ_INSTANCES);

    AwaStaticClient_DefineResource(awaClient, FREQ_OBJID, FREQ_VALUE, "Sensor Value", AwaResourceType_Float, 0, 1, AwaResourceOperations_ReadOnly);
    AwaStaticClient_SetResourceStorageWithPointer(awaClient, FREQ_OBJID, FREQ_VALUE, &freq_obj[0].value, sizeof(freq_obj[0].value), sizeof(freq_obj[0]));
    AwaStaticClient_DefineResource(awaClient, FREQ_OBJID, FREQ_UNITS, "Sensor Units", AwaResourceType_Float, 0, 1, AwaResourceOperations_ReadOnly);
    AwaStaticClient_SetResourceStorageWithPointer(awaClient, FREQ_OBJID, FREQ_UNITS, &freq_obj[0].units, sizeof(freq_obj[0].units), sizeof(freq_obj[0]));
    AwaStaticClient_DefineResource(awaClient, FREQ_OBJID, FREQ_MEASURED_MAX, "Max Measured Value", AwaResourceType_Float, 0, 1, AwaResourceOperations_ReadOnly);
    AwaStaticClient_SetResourceStorageWithPointer(awaClient, FREQ_OBJID, FREQ_MEASURED_MAX, &freq_obj[0].measured_max, sizeof(freq_obj[0].measured_max), sizeof(freq_obj[0]));
    AwaStaticClient_DefineResource(awaClient, FREQ_OBJID, FREQ_MEASURED_MIN, "Min Measured Value", AwaResourceType_Float, 0, 1, AwaResourceOperations_ReadOnly);
    AwaStaticClient_SetResourceStorageWithPointer(awaClient, FREQ_OBJID, FREQ_MEASURED_MIN, &freq_obj[0].measured_min, sizeof(freq_obj[0].measured_min), sizeof(freq_obj[0]));
    AwaStaticClient_DefineResource(awaClient, FREQ_OBJID, FREQ_RANGE_MAX, "Max Range Value", AwaResourceType_Float, 0, 1, AwaResourceOperations_ReadOnly);
    AwaStaticClient_SetResourceStorageWithPointer(awaClient, FREQ_OBJID, FREQ_RANGE_MAX, &freq_obj[0].range_max, sizeof(freq_obj[0].range_max), sizeof(freq_obj[0]));
    AwaStaticClient_DefineResource(awaClient, FREQ_OBJID, FREQ_RANGE_MIN, "Min Range Value", AwaResourceType_Float, 0, 1, AwaResourceOperations_ReadOnly);
    AwaStaticClient_SetResourceStorageWithPointer(awaClient, FREQ_OBJID, FREQ_RANGE_MIN, &freq_obj[0].range_min, sizeof(freq_obj[0].range_min), sizeof(freq_obj[0]));

    AwaStaticClient_DefineResource(awaClient, FREQ_OBJID, FREQ_RESET, "Reset Measured Values", AwaResourceType_None, 0, 1, AwaResourceOperations_Execute);
    AwaStaticClient_SetResourceOperationHandler(awaClient, FREQ_OBJID, FREQ_RESET, handler);

    AwaStaticClient_DefineResource(awaClient, FREQ_OBJID, FREQ_CALIBRATION, "Calibration", AwaResourceType_String, 0, 1, AwaResourceOperations_ReadWrite);
    AwaStaticClient_SetResourceStorageWithPointer(awaClient, FREQ_OBJID, FREQ_CALIBRATION, &freq_obj[0].calibration, sizeof(freq_obj[0].calibration), sizeof(freq_obj[0]));
    AwaStaticClient_DefineResource(awaClient, FREQ_OBJID, FREQ_APP_TYPE, "Application Type", AwaResourceType_String, 0, 1, AwaResourceOperations_ReadWrite);
    AwaStaticClient_SetResourceStorageWithPointer(awaClient, FREQ_OBJID, FREQ_APP_TYPE, &freq_obj[0].app_type, sizeof(freq_obj[0].app_type), sizeof(freq_obj[0]));

    for (instance=0; instance<FREQ_INSTANCES; instance++)
    {
        AwaStaticClient_CreateObjectInstance(awaClient, FREQ_OBJID, instance);

        AwaStaticClient_CreateResource(awaClient, FREQ_OBJID, instance, FREQ_VALUE);
        freq_obj[instance].value = 0.0;
        AwaStaticClient_CreateResource(awaClient, FREQ_OBJID, instance, FREQ_UNITS);
        freq_obj[instance].units = 0.0;
        AwaStaticClient_CreateResource(awaClient, FREQ_OBJID, instance, FREQ_MEASURED_MAX);
        freq_obj[instance].measured_max = 0.0;
        AwaStaticClient_CreateResource(awaClient, FREQ_OBJID, instance, FREQ_MEASURED_MIN);
        freq_obj[instance].measured_min = 10.0;
        AwaStaticClient_CreateResource(awaClient, FREQ_OBJID, instance, FREQ_RANGE_MAX);
        freq_obj[instance].range_max = 0.0;
        AwaStaticClient_CreateResource(awaClient, FREQ_OBJID, instance, FREQ_RANGE_MIN);
        freq_obj[instance].range_min = 0.0;

        AwaStaticClient_CreateResource(awaClient, FREQ_OBJID, instance, FREQ_RESET);

        AwaStaticClient_CreateResource(awaClient, FREQ_OBJID, instance, FREQ_CALIBRATION);
        strncpy(freq_obj[instance].calibration, "12", sizeof(freq_obj[instance].calibration));
        AwaStaticClient_CreateResource(awaClient, FREQ_OBJID, instance, FREQ_APP_TYPE);
        strncpy(freq_obj[instance].app_type, "saw", sizeof(freq_obj[instance].app_type));

    }
}

static void UpdateResources(AwaStaticClient * awaClient)
{
    static int x = 0;
    static float y = 0.0;
    int p = atoi(freq_obj[0].calibration);

    if (!strncmp(freq_obj[0].app_type, "square", strlen(freq_obj[0].app_type)))
    {
        y = (x++ % (p*2)) < p ? p : 0;
    }
    else
    {
        y = p - abs((x++ % (p*2)) - p);
    }
    freq_obj[0].value = y;
    freq_obj[0].range_max = p;
    freq_obj[0].measured_max = (y > freq_obj[0].measured_max) ? y : freq_obj[0].measured_max;
    freq_obj[0].measured_min = (y < freq_obj[0].measured_min) ? y : freq_obj[0].measured_min;

    AwaStaticClient_ResourceChanged(awaClient, FREQ_OBJID, 0, FREQ_VALUE);
    printf("value: %f type: %s\n", freq_obj[0].value, freq_obj[0].app_type);
}

static void printNameservers(void)
{
    int ns;
    for (ns=0 ;; ns++)
    {
        uip_ipaddr_t *r = uip_nameserver_get(ns);
        if (!r)
        {
            break;
        }
        printf("******* DNS Resolver: %04x:%04x:%04x:%04x:%04x:%04x:%04x:%04x *********\n",
            ntohs(r->u16[0]),
            ntohs(r->u16[1]),
            ntohs(r->u16[2]),
            ntohs(r->u16[3]),
            ntohs(r->u16[4]),
            ntohs(r->u16[5]),
            ntohs(r->u16[6]),
            ntohs(r->u16[7]));
    }
}


PROCESS(lwm2m_client, "Awa LWM2M Example Client");
AUTOSTART_PROCESSES(&lwm2m_client);

#ifndef IDENTITY
#error Please set the IDENTITY
#endif

#ifndef SECRET_KEY
#error Please set the SECRET_KEY
#endif

#ifndef BINARY_KEY
#error Please set the BINARY_KEY
#endif

static const unsigned char binaryKey[] = {BINARY_KEY};

PROCESS_THREAD(lwm2m_client, ev, data)
{
    PROCESS_BEGIN();

    static AwaStaticClient * awaClient;

    printf("\n");
    printf("uIP buffer: %u\n", UIP_BUFSIZE);
    printf("LL header: %u\n", UIP_LLH_LEN);
    printf("IP+UDP header: %u\n", UIP_IPUDPH_LEN);

    //printf("%04XIP+UDP header: %u\n", UIP_IPUDPH_LEN);

    awaClient = AwaStaticClient_New();

    srand(time(NULL));
    int port = 6000 + (rand() % 32768);

    // AwaStaticClient_SetLogLevel(AwaLogLevel_Debug);
    AwaStaticClient_SetLogLevel(AwaLogLevel_Verbose);

    AwaStaticClient_SetEndPointName(awaClient, "AwaStaticClient1");
    AwaStaticClient_SetPSK(awaClient, IDENTITY, binaryKey, sizeof(binaryKey));
    AwaStaticClient_SetCoAPListenAddressPort(awaClient, "::", port);
    AwaStaticClient_SetBootstrapServerURI(awaClient, "coaps://deviceserver.flowcloud.systems:15684");
    AwaStaticClient_Init(awaClient);

    uip_ipaddr_t ipaddr;
    uip_ip6addr(&ipaddr, BOOTSTRAP_IPv6_ADDR1, BOOTSTRAP_IPv6_ADDR2, BOOTSTRAP_IPv6_ADDR3,
        BOOTSTRAP_IPv6_ADDR4, BOOTSTRAP_IPv6_ADDR5, BOOTSTRAP_IPv6_ADDR6, BOOTSTRAP_IPv6_ADDR7,
        BOOTSTRAP_IPv6_ADDR8);
#ifdef CONTIKI_TARGET_MIKRO_E
    uip_ds6_defrt_add(&ipaddr, 0);
#endif

    uip_nameserver_update(&ipaddr, UIP_NAMESERVER_INFINITE_LIFETIME);

#ifdef CONTIKI_TARGET_MINIMAL_NET
#warning hardcoded tomfoolery here
    uip_ip6addr(&ipaddr, 0xfdfd, 0, 0, 0, 0, 0, 0, 1);
    uip_ds6_defrt_add(&ipaddr, 0);

    {
        char *ipaddradd = "ip address add fdfd::1/64 dev tap0";
        int ret = system(ipaddradd);
        (void) ret;
        printf("%s\n", ipaddradd);
    }

    printNameservers();
#endif

    DefineFrequencyObject(awaClient);

    static struct etimer et;

    while (1)
    {
        static int waitTime;

        printf("Running the loop\n");
        waitTime = AwaStaticClient_Process(awaClient);

        UpdateResources(awaClient);

        etimer_set(&et, (2 * waitTime * CLOCK_SECOND) / 1000);
        PROCESS_YIELD();
        waitTime = 0;
    }

    AwaStaticClient_Free(&awaClient);

    PROCESS_END();
}
