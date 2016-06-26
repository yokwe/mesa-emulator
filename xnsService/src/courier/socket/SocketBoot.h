//
// SocketBoot.h
//

#ifndef SOCKETBOOT_H__
#define SOCKETBOOT_H__

#include "../SocketManager.h"
#include "../stub/StubDatagram.h"
#include "../stub/StubSequencedPacket.h"


class SocketBoot : public SocketManager::Socket {
public:
	static const quint48 BFN_GVWIN_NETBOOT = 0x0000AA000E60ULL;

	SocketBoot() : SocketManager::Socket("Boot") {}
	void addBootFile(quint48 bfn, QString path) {
		BootFile::add(bfn, path);
	}

	void process(Socket::Context& context, ByteBuffer& request, ByteBuffer& response);

private:
	class BootFile {
	public:
		static void add(quint48 bfn, QString path);
		static BootFile* getInstance(quint48 bfn);

		const quint48 bfn;
		const QString path;

		quint32       size;
		void*         address;

	private:
		static QMap<quint48, BootFile*> map; // key is bfn

		BootFile(quint48 bfn, QString path);
		~BootFile();
	};

	class Connection {
	public:
		static void        add(quint48 host, quint48 bfn);
		static Connection* getInstance(quint48 host);
		static quint16     getLocalID();

		quint48   host;
		BootFile* bootFile;
		quint32   pos;

		Courier::SequencedPacket::Header  header;
	private:
		static QMap<quint48, Connection*> map; // key is host
		static quint16 nextLocalID;

		Connection(quint48 host, quint48 bfn);
	};
};

#endif
