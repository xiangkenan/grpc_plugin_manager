#include <stdlib.h>
#include <iostream>
#include <memory>
#include <string>
#include <grpc++/grpc++.h>

#include "glog/logging.h"
#include "fcgi_stdio.h"
#include "plugin_client.cc"

using namespace std;

#define THREAD_COUNT 1024

void *work(void *i) 
{
    FCGX_Request request;  
    FCGX_InitRequest(&request, 0, 0);  

	plugin_client *client = new plugin_client(grpc::CreateChannel("localhost:9502", 
				grpc::InsecureChannelCredentials()));

    while (true) 
	{
        if (FCGX_Accept_r(&request) < 0) 
		{
            break;
        }

        const string& params = FCGX_GetParam("QUERY_STRING", request.envp);
        Response response;
        client->ChooseClassMethod(params, &response);

        FCGX_FPrintF(request.out,
                "Content-type: text/json\r\n"
                "\r\n"
                "%s", response.result().c_str());

        FCGX_Finish_r(&request);

        LOG(INFO) << "params: " << params << "\t" << "ret: " << response.result();
    }

    return NULL;  
}  

int main(int argc, char** argv) 
{
    FLAGS_log_dir = "./log";
	FLAGS_logbufsecs = 0;
    google::InitGoogleLogging(argv[0]);

    pthread_t id[THREAD_COUNT];
    FCGX_Init();

    for (int i = 0; i < THREAD_COUNT; ++i) 
	{
        pthread_create(&id[i], NULL, work, (void *)&i);
        cout << id[i] << endl;
    }

    void *thread_result;
    for (int i = 0; i < THREAD_COUNT; ++i) 
	{
        pthread_join(id[i], &thread_result);
        cout << "线程失败:" << id[i] << ";" <<  endl;
    }

    return 0;
}
