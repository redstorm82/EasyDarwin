/*
	Copyleft (c) 2012-2016 EasyDarwin.ORG.  All rights reserved.
	Github: https://github.com/EasyDarwin
	WEChat: EasyDarwin
	Website: http://www.EasyDarwin.org
*/
/*!
  \file    HTTPSession.h
  \author  Babosa@EasyDarwin.org
  \date    2014-12-03
  \version 1.0
  \mainpage 使用引导

  网络调用主要流程\n
  Select -> HTTPSession -> MsgCenter -> HTTPSession -> Cleanup\n\n

  Copyright (c) 2014 EasyDarwin.org 版权所有\n

  \defgroup 服务单元网络事件处理流程
*/

#ifndef __HTTPSESSION_H__
#define __HTTPSESSION_H__

#include "HTTPSessionInterface.h"
#include "HTTPRequest.h"
#include "QTSSModule.h"

class HTTPSession : public HTTPSessionInterface
{
public:
	HTTPSession();
	virtual ~HTTPSession();

	////发送HTTP响应报文
	virtual QTSS_Error SendHTTPPacket(StrPtrLen* contentXML, bool connectionClose, bool decrement);

private:
	SInt64 Run();

	// Does request prep & request cleanup, respectively
	QTSS_Error SetupRequest();
	void CleanupRequest();

	QTSS_Error ExecNetMsgEasyHLSModuleReq(char* queryString, char* json);
	QTSS_Error ExecNetMsgGetHlsSessionsReq(char* queryString, char* json);
	QTSS_Error ExecNetMsgGetRTSPPushSessionsReq(char* queryString, char* json);

	// test current connections handled by this object against server pref connection limit
	bool OverMaxConnections(UInt32 buffer);

	HTTPRequest*        fRequest;
	OSMutex             fReadMutex;

	//网络报文处理状态机
	enum
	{
		kReadingRequest = 0,	//读取报文
		kFilteringRequest = 1,	//过滤报文
		kPreprocessingRequest = 2,	//预处理报文
		kProcessingRequest = 3,	//处理报文
		kSendingResponse = 4,	//发送响应报文
		kCleaningUp = 5,	//清空本次处理的报文内容

		kReadingFirstRequest = 6,	//第一次读取Session报文，主要用来做Session协议区分（HTTP/TCP/RTSP等等）
		kHaveCompleteMessage = 7    // 读取到完整的报文
	};

	UInt32 fCurrentModule;
	UInt32 fState;

	QTSS_RoleParams     fRoleParams;//module param blocks for roles.
	QTSS_ModuleState    fModuleState;

	//清空请求报文
	QTSS_Error DumpRequestData();

};
#endif // __HTTPSESSION_H__

