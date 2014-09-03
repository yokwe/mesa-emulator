/*
Copyright (c) 2014, Yasuhiro Hasegawa
All rights reserved.

Redistribution and use in source and binary forms, with or without modification,
are permitted provided that the following conditions are met:

  1. Redistributions of source code must retain the above copyright notice,
     this list of conditions and the following disclaimer.
  2. Redistributions in binary form must reproduce the above copyright notice,
     this list of conditions and the following disclaimer in the documentation
     and/or other materials provided with the distribution.
  3. The name of the author may not be used to endorse or promote products derived
     from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES,
INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY
DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA,
OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY
OF SUCH DAMAGE.
*/


//
// AgentNetwork.h
//

#ifndef AGENTNETWORK_H__
#define AGENTNETWORK_H__

#include "Agent.h"
#include "NetworkPacket.h"

class AgentNetwork : public Agent {
public:
	class TransmitThread: public QRunnable {
	public:
		// Wait interval in milliseconds for QWaitCondition::wait
		static const int WAIT_INTERVAL = 1000;
		static const QThread::Priority PRIORITY = QThread::HighPriority;

		static void stop() {
			stopThread = 1;
		}

		TransmitThread() {
			stopThread        = 0;
			interruptSelector = 0;
			networkPacket     = 0;
		}

		void setInterruptSelector(CARD16 interruptSelector_) {
			interruptSelector = interruptSelector_;
		}
		void setNetworkPacket(NetworkPacket* networkPacket_) {
			networkPacket = networkPacket_;
		}

		void enqueue(EthernetIOFaceGuam::EthernetIOCBType* iocb);

		void run();
		void reset();

		quint64 getSec();

	private:
		class Item {
		public:
			EthernetIOFaceGuam::EthernetIOCBType* iocb;

			Item(EthernetIOFaceGuam::EthernetIOCBType* iocb_) : iocb(iocb_) {}
			Item(const Item& that) : iocb(that.iocb) {}
		};

		static int        stopThread;

		CARD16            interruptSelector;
		NetworkPacket*    networkPacket;

		QMutex            transmitMutex;
		QWaitCondition    transmitCV;
		QLinkedList<Item> transmitQueue;
	};
	class ReceiveThread: public QRunnable {
	public:
		static const CARD32 MAX_WAIT_SEC = 40;
		static const QThread::Priority PRIORITY = QThread::HighPriority;

		static void stop() {
			stopThread = 1;
		}

		ReceiveThread() {
			stopThread        = 0;
			interruptSelector = 0;
			networkPacket     = 0;
		}

		void setInterruptSelector(CARD16 interruptSelector_) {
			interruptSelector = interruptSelector_;
		}
		void setNetworkPacket(NetworkPacket* networkPacket_) {
			networkPacket = networkPacket_;
		}

		void enqueue(EthernetIOFaceGuam::EthernetIOCBType* iocb);

		void run();
		void reset();

		quint64 getSec();

	private:
		class Item {
		public:
			// queued time in second
			qint64                                sec;
			EthernetIOFaceGuam::EthernetIOCBType* iocb;

			Item(qint64 sec_, EthernetIOFaceGuam::EthernetIOCBType* iocb_) : sec(sec_), iocb(iocb_) {}
			Item(const Item& that) : sec(that.sec), iocb(that.iocb) {}
		};

		static int        stopThread;

		CARD16            interruptSelector;
		NetworkPacket*    networkPacket;

		QMutex            receiveMutex;
		QLinkedList<Item> receiveQueue;
	};

	ReceiveThread  receiveThread;
	TransmitThread transmitThread;

	AgentNetwork() : Agent(GuamInputOutput::network, "Network") {
		fcb = 0;
		networkPacket = 0;
	}

	CARD32 getFCBSize() {
		return SIZE(*fcb);
	}

	void Initialize();
	void Call();

	void poll();

	void setNetworkPacket(NetworkPacket* networkPacket_) {
		networkPacket = networkPacket_;
	}

private:
	EthernetIOFaceGuam::EthernetFCBType* fcb;
	NetworkPacket*                       networkPacket;
};

#endif
