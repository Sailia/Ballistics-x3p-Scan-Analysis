#include "elzip.hpp"

namespace fs = std::experimental::filesystem;

namespace elz
{
    bool extractZip(bool displayDebug, std::string zipname, std::string target)
    {
        ziputils::unzipper zipFile;
        bool isOpen = zipFile.open(zipname.c_str());
		if (isOpen) {
			for (std::string filename : zipFile.getFilenames())
			{
				fs::path cDir(target + ((fs::path(filename).parent_path().string() == "") ? "" : "/") + fs::path(filename).parent_path().string());
				fs::path cFile(target + "/" + filename);
				if (displayDebug) {
					std::cout << "Path : " << cDir.string() << std::endl;
					std::cout << "Exist : " << fs::exists(cDir) << std::endl;
					std::cout << "ExistF : " << fs::exists(cFile) << std::endl;
					std::cout << "IsDir : " << fs::is_directory(cDir) << std::endl;
				}
				fs::path fillPath;
				for (fs::path pathPart : cDir) {
					fillPath /= pathPart;
					if (!fs::exists(fillPath)) {
						if (displayDebug) {
							std::cout << "Creating directory : " << cDir.string() << std::endl;
						}
						fs::create_directory(fillPath);
					}
				}
				if (displayDebug) {
					std::cout << "Creating file : " << cFile.string() << std::endl;
				}
				zipFile.openEntry(filename.c_str());
				std::ofstream wFile;
				wFile.open(cFile.string(), std::ostream::out | std::ostream::binary);
				zipFile >> wFile;
				wFile.close();
			}
		}
		return isOpen;
    }
}