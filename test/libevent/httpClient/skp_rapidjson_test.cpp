#include "skp_rapidjson_test.h"
#include <stdio.h>

#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"

#include <iostream>

using namespace rapidjson;

void skp_rapidjson_test() {
    // 1. 把JSON解析至DOM。
    const char* json = "{\"project\":\"rapidjson\",\"stars\":10}";
    Document d;
    d.Parse(json);
    // 2. 利用DOM作出修改。
    Value& s = d["stars"];
    s.SetInt(s.GetInt() + 1);
    // 3. 把DOM转换（stringify）成JSON。
    StringBuffer buffer;
    Writer<StringBuffer> writer(buffer);
    d.Accept(writer);
    // Output {"project":"rapidjson","stars":11}
    std::cout << buffer.GetString() << std::endl;
}

void skp_rapidjson_test_2()
{
    //char json[100] = "{ \"hello\" : \"world\" }";

    const char json[] = " { \"hello\" : \"world\", \"t\" : true , \"f\" : false, \"n\": null, \"i\":123, \"pi\": 3.1416, \"a\":[1, 2, 3, 4] } ";

    rapidjson::Document d;
    d.Parse<0>(json);
    if (d.HasParseError())
    {
        printf("GetParseError %s\n",d.GetParseError());
    }

    if (d.HasMember("a"))//这个时候要保证d湿IsObject类型 最好是 if(d.Isobject() && d.HasMember("a"))
    {
        const Value &a=d["a"];
        if (a.IsArray() && a.Size() > 3)
        {
            const Value &a3=a[2];
            if (a3.IsInt())
            {
                    printf("GetInt [%d] \n",a3.GetInt()); ;
            }
        }
    }
}

void skp_rapidjson_test_3()
{
    rapidjson::Document document;
    rapidjson::Document::AllocatorType& allocator = document.GetAllocator();
    rapidjson::Value root(rapidjson::kObjectType);
    root.AddMember("name", "哥伦布啊", allocator);
    root.AddMember("gold",1234,allocator);
    rapidjson::StringBuffer buffer;
    rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
    root.Accept(writer);
    std::string reststring = buffer.GetString();
    std::cout << reststring << std::endl;
}

void skp_rapidjson_test_4()
{
    //read json
    std::string updateInfo = "{\"UpdateInfo\":[{\"url\":\"aaaa.ipa\",\"platform\":\"ios\"}]}";

    rapidjson::Document doc;
    doc.Parse<0>(updateInfo.c_str());

    rapidjson::Value &dataArray = doc["UpdateInfo"];

    if (dataArray.IsArray())
    {
        for (int i = 0; i < dataArray.Size(); i++)
        {
            const rapidjson::Value& object = dataArray[i];

            std::string url = object["url"].GetString();
            std::string platform = object["platform"].GetString();
            std::cout << url << platform << std::endl;
        }
    }



    //write json
    rapidjson::Document document;
    document.SetObject();
    rapidjson::Document::AllocatorType& allocator = document.GetAllocator();
    rapidjson::Value array(rapidjson::kArrayType);
    rapidjson::Value object(rapidjson::kObjectType);
    object.AddMember("id", 1, allocator);
    object.AddMember("name", "test", allocator);
    object.AddMember("version", 1.01, allocator);
    object.AddMember("vip", true, allocator);
    array.PushBack(object, allocator);

    rapidjson::Value array2(rapidjson::kArrayType);

    for (int i = 0; i < 10; i++)
    {
        rapidjson::Value object(rapidjson::kObjectType);
        object.SetInt(i);
        array2.PushBack(object, allocator);
    }
    rapidjson::Value object2(rapidjson::kObjectType);
    object2.AddMember("array", array2, allocator);
    array.PushBack(object2, allocator);

    document.AddMember("title", "PLAYER INFO", allocator);
    document.AddMember("array", array, allocator);

    rapidjson::StringBuffer buffer;
    rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
    document.Accept(writer);
    std::string str = buffer.GetString();
    std::cout << str << std::endl;
}
