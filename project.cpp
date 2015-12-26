#include <cpprest/json.h>
#include <cpprest/http_client.h>
#include <iostream>

using namespace std;
using namespace web::http;
using namespace web::http::client;

typedef web::json::value JsonValue;
typedef web::json::value::value_type JsonValueType;
typedef std::wstring String;
typedef std::wstringstream StringStream;



class Answer
{
public:
	int Id;
	String Question;
	String Author;
	String AnswerText;
	String Date;

	void Display() const
	{
		wcout << "		ID: " << Id << endl;
		wcout << "		Question: " << Question << endl;
		wcout << "		Author: " << Author << endl;
		wcout << "		AnswerText: " << AnswerText << endl;
		wcout << "		Date: " << Date << endl;
		wcout << endl;
	}
};

class Question
{
public:
	int Id;
	String Address;
	String Author;
	String Title;
	String QuestionText;
	String Date;
	std::list<Answer> Answers;


	void Display() const
	{
		wcout << "ID: " << Id << endl;
		wcout << "Address: " << Address << endl;
		wcout << "Author: " << Author << endl;
		wcout << "Question: " << QuestionText << endl;
		wcout << "Title: " << Title << endl;
		wcout << "Date: " << Date << endl;
		wcout << endl;
	}
};

class ProtocolBase
{
public:
	virtual ~ProtocolBase() {}

	virtual void requestQuestions(const String&, std::function<void(std::vector<Question>)>) = 0;
	virtual void requestAnswersForQuestion(const String&, const Question&, std::function<void(std::list<Answer>)>) = 0;
};

class Casablanca : public ProtocolBase
{
public:
	void requestQuestions(const String& apiAddress, std::function<void(std::vector<Question>)> completionHandler) override
	{
		pplx::create_task([apiAddress, completionHandler]
		{
			http_client client(apiAddress);
			return client.request(methods::GET, L"");
		})
			.then([completionHandler](http_response response)
		{
			if (response.status_code() == status_codes::OK)
			{
				std::vector<Question> questions;

				auto json = response.extract_json().get();

				if (json.is_null())
					return;

				for (size_t i = 0; i < json.size(); i++)
				{
					Question question;
					question.Id = json[i].at(L"id").as_integer();
					question.Address = json[i].at(L"url").as_string();
					question.Author = json[i].at(L"author").as_string();
					question.Title = json[i].at(L"question_title").as_string();
					question.QuestionText = json[i].at(L"question_text").as_string();
					question.Date = json[i].at(L"pub_date").as_string();

					questions.push_back(question);
				}

				completionHandler(questions);
			}
			else
			{
				throw "Invalid server response";
			}
		}).wait();
	}
	void requestAnswersForQuestion(const String& apiAddress, const Question& question, std::function<void(std::list<Answer>)> completionHandler) override
	{
		pplx::create_task([apiAddress, completionHandler, question]
		{
			http_client client(apiAddress);
			return client.request(methods::GET, L"?question=" + to_wstring(question.Id));
		})
			.then([completionHandler](http_response response)
		{
			if (response.status_code() == status_codes::OK)
			{
				std::list<Answer> answers;
				auto json = response.extract_json().get();

				if (json.is_null())
					return;

				for (size_t i = 0; i < json.size(); i++)
				{
					Answer answer;
					answer.Id = json[i].at(L"id").as_integer();
					answer.Author = json[i].at(L"author").as_string();
					answer.Question = json[i].at(L"question").as_string();
					answer.AnswerText = json[i].at(L"answer_text").as_string();
					answer.Date = json[i].at(L"pub_date").as_string();

					answers.push_back(answer);
				}

				completionHandler(answers);
			}
			else
			{
				throw "Invalid server response";
			}
		}).wait();

	}
};

class Protocol
{
	static ProtocolBase* protocol;

public:
	static void requestQuestions(const String& apiAddress, std::function<void(std::vector<Question>)> completionHandler)
	{
		protocol->requestQuestions(apiAddress, completionHandler);
	};
	static void requestAnswersForQuestion(const String& apiAddress, const Question& question, std::function<void(std::list<Answer>)> completionHandler)
	{
		protocol->requestAnswersForQuestion(apiAddress, question, completionHandler);
	}
};

ProtocolBase* Protocol::protocol = new Casablanca();

std::vector<Question> questions;

int main()
{
	std::locale loc("ru");
	std::cout.imbue(loc);
	std::wcout.imbue(loc);

	while (1)
	{
		Protocol::requestQuestions(L"http://devernua.pythonanywhere.com/qs/", [](std::vector<Question> q)
		{
			for (auto question = q.begin(); question != q.end(); ++question)
			{
				auto found = false;
				Question* originalQuestion;
				for (auto oQuestion = questions.begin(); oQuestion != questions.end(); ++oQuestion)
				{
					if (question->Id == oQuestion->Id)
					{
						found = true;
						originalQuestion = &(*oQuestion);
						break;
					}
				}
				if (!found)
				{
					question->Display();
					Protocol::requestAnswersForQuestion(L"http://devernua.pythonanywhere.com/as/", *question, [&question](std::list<Answer> a)
					{
						question->Answers = a;
						for (auto answer = question->Answers.begin(); answer != question->Answers.end(); ++answer)
						{
							answer->Display();
						}
					});
					questions.push_back(*question);
				}
				else
				{
					Protocol::requestAnswersForQuestion(L"http://devernua.pythonanywhere.com/as/", *originalQuestion, [originalQuestion](std::list<Answer> a)
					{
						if (a.size() != 0)
							for (auto answer = a.begin(); answer != a.end(); ++answer)
							{
								auto nFound = false;
								for (auto oAnswer = originalQuestion->Answers.begin(); oAnswer != originalQuestion->Answers.end(); ++oAnswer)
								{
									if (answer->Id == oAnswer->Id)
									{
										nFound = true;
										break;
									}
								}
								if (!nFound)
								{
									originalQuestion->Answers.push_back(*answer);
									answer->Display();
								}
							}
					});
				}
			}
		});
		std::this_thread::sleep_for(std::chrono::milliseconds(10000));
	}
}