#include <iostream>
#include <string>
#include <fstream>

//checks if file exits
inline bool Exists (const std::string& name) {
  struct stat buffer;   
  return (stat (name.c_str(), &buffer) == 0); 
}

class net{
    private:
        //resposne of curl
        std::string Data;

        //status code
        std::string Code;
    public:
        //Get request
        void Get(std::string url)
        {
            //get url
            const std::string command = "curl -G -s -o temp.tmp -w \"%{http_code}\n\" "+url+" > temp2.tmp";
            system(command.c_str());

            std::ifstream file("temp.tmp");
            std::ifstream status("temp2.tmp");
            
            //get status code
            status.close();
            getline(status,this->Code);

            this->Data = "";
            std::string line;

            //add response to data var
            while(getline(file,line))
            {
                this->Data += line;
            }

            //close files and delete them
            file.close();
            //remove("temp.tmp");
            //remove("temp2.tmp");
        }

        //Post request
        void Post(std::string url, std::string data)
        {
            //Post url
            const std::string command = "curl -d "+data+" -s -o temp.tmp -w \"%{http_code}\n\" "+url+" > temp2.tmp";
            system(command.c_str());


            //wait till files are made
            while(!Exists("temp.tmp"))
            {

            }

            while(!Exists("temp2.tmp"))
            {

            }

            std::ifstream file("temp.tmp");
            std::ifstream status("temp2.tmp");

            //get status code
            getline(status,this->Code);

            this->Data = "";
            std::string line;

            //add response to data var
            while(getline(file,line))
            {
                this->Data += line;
            }

            //close files and delete them
            file.close();
            status.close();
            remove("temp.tmp");
            remove("temp2.tmp");

        }
        
        //Response accsessor
        std::string Response()
        {
            return this->Data;
        }

        //Status Code accsessor
        std::string StatusCode()
        {
            return this->Code;
        }
};
