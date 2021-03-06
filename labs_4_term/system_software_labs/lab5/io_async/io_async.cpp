#include "stdafx.h"
#include "io_async.h"
#include <Windows.h>
#include <boost/filesystem.hpp>
#include <conio.h>
#include <sstream>
#include <filesystem>
#include <iostream>

using namespace std;

namespace io
{
	vector<string> get_all_files_names_within_folder(const string& folder)
	{
		vector<string> names;

		const std::string path = ".\\" + folder;
		for (auto & p : std::experimental::filesystem::directory_iterator(path))
			names.emplace_back(p.path().string());
		return names;
	}

	DWORD WINAPI io_async::begin_read(LPVOID parameter)
	{
		io_async* instance = static_cast<io_async*>(parameter);
		for (const auto& filename : instance->filenames_)
		{
			//Sleep(500);
			std::ifstream file(filename);
			std::stringstream buffer;
			buffer << file.rdbuf();
			std::string * file_data = new std::string(buffer.str());
			std::cout << "File " << filename << " data: " << *file_data << endl;
			instance->queue_->push(file_data);
			SetEvent(instance->events_for_waiting[0]);
			file.close();
		}
		SetEvent(instance->events_for_waiting[1]);
		return 0;
	}

	DWORD WINAPI io_async::begin_write(LPVOID parameter)
	{
		io_async* instance = static_cast<io_async*>(parameter);
		while (true)
		{
			const auto handle_id = WaitForMultipleObjects(2, instance->events_for_waiting, FALSE, INFINITE);
			if (handle_id == 0)
			{
				ofstream file(instance->output_file_path_, std::ios::app);
				std::string * input_data = nullptr;
				while (instance->queue_->pop(input_data))
				{
					file << *input_data;
				}
				file.close();
			}
			if (handle_id == 1)
			{
				break;
			}
		}
		return 0;
	}

	io_async::io_async()
	{
		this->queue_ = new boost::lockfree::queue<std::string*, boost::lockfree::capacity<50>>;
		this->events_for_waiting = new HANDLE[2];
		this->events_for_waiting[0] = CreateEvent(NULL, FALSE, FALSE, L"DataToWriteExists");
		this->events_for_waiting[1] = CreateEvent(NULL, FALSE, FALSE, L"ReadingComplete");
	}

	int io_async::concat_files(std::string source_folder_path, std::string output_file_path)
	{
		this->source_folder_path_ = source_folder_path;
		this->output_file_path_ = output_file_path;
		this->filenames_ = get_all_files_names_within_folder(this->source_folder_path_);

		this->reader_thread_handle_ = CreateThread(NULL, NULL, begin_read, static_cast<LPVOID>(this), NULL, NULL);
		this->writer_thread_handle_ = CreateThread(NULL, NULL, begin_write, static_cast<LPVOID>(this), NULL, NULL);

		return 0;
	}
}

