#include <iostream>
#include <iomanip>
using namespace std;


#include "hksdk\hkobjectfactory.h"

using namespace hksdk;
using namespace std;


void test_ipc(hkobjectfactory &factory)
{
	cout << "����ʵʱԤ��" << endl;
	shared_ptr<hknvr> nvr = factory.get_nvr("192.168.1.64");
	if (!nvr)
		return;

	shared_ptr<hkipc> ipc;
	ipc = factory.create_ipc(nvr, "192.168.254.3", "admin", "admin1234");
	ipc->StartRealPlay(GetConsoleWindow());


	Sleep(100000);
}

void test_download_by_time(hkobjectfactory &factory)
{
	cout << "���԰���ʱ������¼��" << endl;
	shared_ptr<hknvr> nvr = factory.get_nvr("192.168.1.64");
	if (!nvr)
		return;

	shared_ptr<hkvideodownload> download = factory.create_download(
		nvr,
		"192.168.254.3",
		"2020-05-26 10:00:00",
		"2020-05-26 10:10:00",
		"d:\\123.mp4");

	download->StartDownload();
	
	while (download->GetProgress() != 100)
	{
		cout << "���ؽ��� " << download->GetProgress() << '\r';
		Sleep(1000);
	}
	cout << "���ؽ��� " << download->GetProgress() << endl;
}

void test_download_by_name(hkobjectfactory &factory)
{
	cout << "���԰����ļ�������¼��" << endl;
	shared_ptr<hknvr> nvr = factory.get_nvr("192.168.1.64");
	if (!nvr) 
		return;

	std::list<FILE_RECORD_INFO> file_list;

	nvr->SearchPlayback(
		list<string>({ "192.168.254.3" }),
		"2020-05-26 10:00:00",
		"2020-05-26 10:30:00",
		file_list
	);


	shared_ptr<hkvideodownload> download = factory.create_download(nvr, file_list.begin()->file_name, "d:\\abc.mp4");
	download->StartDownload();

	while (download->GetProgress() != 100)
	{
		cout << "���ؽ��� " << download->GetProgress()<< '\r';
		Sleep(1000);
	}
	cout << "���ؽ��� " << download->GetProgress() << endl;
}

void test_playback_byname(hkobjectfactory &factory)
{
	cout << "���԰����ļ���������ʷ��Ƶ" << endl;
	shared_ptr<hknvr> nvr = factory.get_nvr("192.168.1.64");
	if (!nvr)
		return;

	std::list<FILE_RECORD_INFO> file_list;
	nvr->SearchPlayback(
		list<string>({ "192.168.254.3" }),
		"2020-05-26 10:00:00",
		"2020-05-26 10:30:00",
		file_list
	);

	shared_ptr<hkvideoplayback> playback = factory.create_playback(nvr, file_list.begin()->file_name);

	playback->StartPlayback(GetConsoleWindow());

	while (!playback->PlaybackDone())
	{
		cout << playback->GetCurrentFrame() << "/" << playback->GetTotalFrames() << '\r';
	}
	cout << playback->GetCurrentFrame() << "/" << playback->GetTotalFrames() << '\r';
}

void test_playback_bytime(hkobjectfactory &factory)
{
	cout << "���԰���ʱ�䲥����ʷ��Ƶ" << endl;
	shared_ptr<hknvr> nvr = factory.get_nvr("192.168.1.64");
	if (!nvr)
		return;

	
	shared_ptr<hkvideoplayback> playback = factory.create_playback(nvr, 
		"192.168.254.3",
		"2020-05-26 10:00:00",
		"2020-05-26 10:00:20");

	playback->StartPlayback(GetConsoleWindow());

	while (!playback->PlaybackDone())
	{
		cout << playback->GetCurrentFrame() << "/" << playback->GetTotalFrames() << '\r';
	}
	cout << playback->GetCurrentFrame() << "/" << playback->GetTotalFrames() << '\r';
}

int main()
{
	hksdk::sdk_init();

	// ��¼NVR
	hkobjectfactory factory;
	factory.login_nvr("192.168.1.64", "admin", "admin1234");

	test_ipc(factory);
	test_download_by_time(factory);
	test_download_by_name(factory);
	test_playback_bytime(factory);
	test_playback_byname(factory);
	
	hksdk::sdk_cleanup();
	return 0;
}
