#include <cpprest/json.h>
#include <cpprest/http_client.h>
#include <iostream>
#include <map>

using namespace std;
using namespace web::http;
using namespace web::http::client;

typedef web::json::value JsonValue;
typedef web::json::value::value_type JsonValueType;
typedef std::wstring String;
typedef std::wstringstream StringStream;

int id = 0;

class Question
{
public:
	int Id;
	String Address;
	String Author;
	String Title;
	String QuestionText;
	String Date;

	void Display() const
	{
		wcout << "ID: " << Id << endl;
		wcout << "Address: " << Address << endl;
		wcout << "Author: " << Author << endl;
		wcout << "Question: " << Title << endl;
		wcout << "Title: " << QuestionText << endl;
		wcout << "Date: " << Date << endl;
		wcout << endl;
	}
};

class QuestionFactory
{
	enum FieldId { id, url, author, question_title, question_text, pub_date	};
	map<String, FieldId> fieldMap_;
	Question question_;

	void SetField(const String& name, const JsonValue& value)
	{
		auto pair = fieldMap_.find(name);
		if (pair != fieldMap_.end())
		{
			switch (pair->second)
			{
			case FieldId::id: 				question_.Id 			= value.as_integer(); 	break;
			case FieldId::url: 				question_.Address 		= value.as_string(); 	break;
			case FieldId::author: 			question_.Author 		= value.as_string(); 	break;
			case FieldId::question_title: 	question_.Title 		= value.as_string(); 	break;
			case FieldId::question_text: 	question_.QuestionText 	= value.as_string(); 	break;
			case FieldId::pub_date: 		question_.Date 			= value.as_string(); 	break;
			default: break;
			}
		}
	}

public:
	QuestionFactory()
	{
		fieldMap_[L"id"] = FieldId::id;
		fieldMap_[L"url"] = FieldId::url;
		fieldMap_[L"author"] = FieldId::author;
		fieldMap_[L"question_title"] = FieldId::question_title;
		fieldMap_[L"question_text"] = FieldId::question_text;
		fieldMap_[L"pub_date"] = FieldId::pub_date;
	}

	Question FromJson(const JsonValue& jsonValue)
	{
		for (auto it = jsonValue.as_object().cbegin(); it != jsonValue.as_object().cend(); ++it)
		{
			auto propertyName = it->first;
			auto propertyValue = it->second;

			SetField(propertyName, propertyValue);
		}

		return question_;
	}
};

pplx::task<void> RequestAsync(const String& apiAddress)
{
	return pplx::create_task([apiAddress]
	{
		http_client client(apiAddress);
		return client.request(methods::GET, L"");
	})
		.then([](http_response response)
	{
		if (response.status_code() == status_codes::OK)
		{
			auto json = response.extract_json().get();

			if (json.is_null())
				return;
			
			for (auto i = 0; i < json.size(); i++)
			{
				if (id < json[i].at(L"id").as_integer())
				{
					id = json[i].at(L"id").as_integer();
					auto question = QuestionFactory().FromJson(json[i]);
					question.Display();
				}
			}
		}
	});
}

int main()
{
	std::locale loc("ru");
	std::cout.imbue(loc);
	std::wcout.imbue(loc);

	while (1)
	{
		RequestAsync(L"http://devernua.pythonanywhere.com/qs/").wait();
		std::this_thread::sleep_for(std::chrono::milliseconds(5000));
	}
}