#include <iostream>
#include <memory>
#include <string>

#include <grpc++/grpc++.h>

#include "ofo_main.grpc.pb.h"
#include "core.h"

class AsyncPlugin final
{
	public:
		bool Run(int argc, char *argv[])
		{
			std::string server_address("0.0.0.0:9502");
			grpc::ServerBuilder builder;
			builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());

			
			PluginManager *service_plugins_ = new PluginManager();
			//插件管理框架
			if(service_plugins_->Init(argc, argv) == false)
			{
				LOG(ERROR) << "manager init error";
				return false;
			}
			
			builder.RegisterService(&service_);
			cq_ = builder.AddCompletionQueue();
			server_ = builder.BuildAndStart();

			HandleRpcs(service_plugins_);
		}
	private:
		class CallData
		{
			public:
				CallData(plugins_rpc::AsyncService* service, grpc::ServerCompletionQueue* cq)
					: service_(service), cq_(cq), responder_(&ctx_), status_(CREATE) 
				{
					Proceed();
				}
				void Proceed(PluginManager* service_plugins_ = NULL) 
				{
					if (status_ == CREATE) 
					{
						status_ = PROCESS;
						service_->RequestChooseClassMethod(&ctx_, &request_, &responder_, cq_, cq_, this);
					} 
					else if (status_ == PROCESS) 
					{
						new CallData(service_, cq_);
						service_plugins_->ChooseClassMethod(&request_, &reply_);
						status_ = FINISH;
						responder_.Finish(reply_, grpc::Status::OK, this);
					} 
					else 
					{
						if(status_ == FINISH)
							delete this;
					}
				}
			private:
				plugins_rpc::AsyncService* service_;
				grpc::ServerCompletionQueue* cq_;
				grpc::ServerContext ctx_;
				Request request_;
				Response reply_;
				grpc::ServerAsyncResponseWriter<Response> responder_;
				enum CallStatus { CREATE, PROCESS, FINISH };
				CallStatus status_;  // The current serving state.
		};
		void HandleRpcs(PluginManager* service_plugins_)
		{
			new CallData(&service_, cq_.get());
			void* tag;  // uniquely identifies a request.
			bool ok;
			while (true) 
			{
				if(cq_->Next(&tag, &ok) && ok)
				{
					static_cast<CallData*>(tag)->Proceed(service_plugins_);
				}
			}
		}

		std::unique_ptr<grpc::ServerCompletionQueue> cq_;
		plugins_rpc::AsyncService service_;
		std::unique_ptr<grpc::Server> server_;
};

int main(int argc, char *argv[])
{
	AsyncPlugin server;
	server.Run(argc, argv);
}
