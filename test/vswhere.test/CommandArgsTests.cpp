// <copyright file="CommandArgsTests.cpp" company="Microsoft Corporation">
// Copyright (C) Microsoft Corporation. All rights reserved.
// Licensed under the MIT license. See LICENSE.txt in the project root for license information.
// </copyright>

#include "stdafx.h"

using namespace std;
using namespace Microsoft::VisualStudio::CppUnitTestFramework;

TEST_CLASS(CommandArgsTests)
{
public:
    TEST_METHOD(Parse_Empty_Defaults)
    {
        CommandArgs args;
        args.Parse(L"vswhere.exe");

        Assert::AreEqual(L"vswhere.exe", args.get_Path().c_str());
        Assert::IsFalse(args.get_All());
        Assert::AreEqual<size_t>(0, args.get_Version().length());
        Assert::IsFalse(args.get_Latest());
        Assert::AreEqual(L"text", args.get_Format().c_str());
        Assert::IsFalse(args.get_Help());
        Assert::IsTrue(args.get_Logo());

        auto& products = args.get_Products();
        Assert::AreEqual<size_t>(3, products.size());
        Assert::IsFalse(find(products.begin(), products.end(), L"Microsoft.VisualStudio.Product.Enterprise") == products.end());
        Assert::IsFalse(find(products.begin(), products.end(), L"Microsoft.VisualStudio.Product.Professional") == products.end());
        Assert::IsFalse(find(products.begin(), products.end(), L"Microsoft.VisualStudio.Product.Community") == products.end());

        auto& requires = args.get_Requires();
        Assert::AreEqual<size_t>(0, requires.size());
    }

    TEST_METHOD(Parse_All)
    {
        CommandArgs args;
        Assert::IsFalse(args.get_All());

        args.Parse(L"vswhere.exe -all");
        Assert::IsTrue(args.get_All());
    }

    TEST_METHOD(Parse_Products_Empty)
    {
        CommandArgs args;

        Assert::ExpectException<win32_error>([&] { args.Parse(L"vswhere.exe -products"); });
    }

    TEST_METHOD(Parse_Products)
    {
        CommandArgs args;

        args.Parse(L"vswhere.exe -products A B");

        auto& products = args.get_Products();
        Assert::AreEqual<size_t>(2, products.size());
        Assert::IsFalse(find(products.begin(), products.end(), L"A") == products.end());
        Assert::IsFalse(find(products.begin(), products.end(), L"B") == products.end());
    }

    TEST_METHOD(Parse_Requires_Empty)
    {
        CommandArgs args;

        Assert::ExpectException<win32_error>([&] { args.Parse(L"vswhere.exe -requires"); });
    }

    TEST_METHOD(Parse_Requires)
    {
        CommandArgs args;

        args.Parse(L"vswhere.exe -requires A B");

        auto& requires = args.get_Requires();
        Assert::AreEqual<size_t>(2, requires.size());
        Assert::IsFalse(find(requires.begin(), requires.end(), L"A") == requires.end());
        Assert::IsFalse(find(requires.begin(), requires.end(), L"B") == requires.end());
    }

    TEST_METHOD(Parse_Version_EOL)
    {
        CommandArgs args;

        Assert::ExpectException<win32_error>([&] { args.Parse(L"vswhere.exe -version"); });
    }

    TEST_METHOD(Parse_Version_Missing)
    {
        CommandArgs args;

        Assert::ExpectException<win32_error>([&] { args.Parse(L"vswhere.exe -version -latest"); });
    }

    TEST_METHOD(Parse_Version)
    {
        CommandArgs args;
        Assert::AreEqual(L"", args.get_Version().c_str());

        args.Parse(L"vswhere.exe -version \"[15.0,16.0)\"");
        Assert::AreEqual(L"[15.0,16.0)", args.get_Version().c_str());
    }

    TEST_METHOD(Parse_Version_Param_Next)
    {
        CommandArgs args;

        Assert::ExpectException<win32_error>([&] { args.Parse(L"vswhere.exe -version -nologo"); });
    }

    TEST_METHOD(Parse_Latest)
    {
        CommandArgs args;
        Assert::IsFalse(args.get_Latest());

        args.Parse(L"vswhere.exe -latest");
        Assert::IsTrue(args.get_Latest());
    }

    TEST_METHOD(Parse_Format_EOL)
    {
        CommandArgs args;

        Assert::ExpectException<win32_error>([&] { args.Parse(L"vswhere.exe -format"); });
    }

    TEST_METHOD(Parse_Format_Missing)
    {
        CommandArgs args;

        Assert::ExpectException<win32_error>([&] { args.Parse(L"vswhere.exe -format -latest"); });
    }

    TEST_METHOD(Parse_Format)
    {
        CommandArgs args;
        Assert::AreEqual(L"text", args.get_Format().c_str());

        args.Parse(L"vswhere.exe -format json");
        Assert::AreEqual(L"json", args.get_Format().c_str());
    }

    TEST_METHOD(Parse_Format_Invalid)
    {
        CommandArgs args;

        Assert::ExpectException<win32_error>([&] { args.Parse(L"vswhere.exe -format invalid"); });
    }

    TEST_METHOD(Parse_NoLogo)
    {
        CommandArgs args;
        Assert::IsTrue(args.get_Logo());

        args.Parse(L"vswhere.exe -nologo");
        Assert::IsFalse(args.get_Logo());
    }

    TEST_METHOD(Parse_Help_Short)
    {
        CommandArgs args;
        Assert::IsFalse(args.get_Help());

        args.Parse(L"vswhere.exe -h");
        Assert::IsTrue(args.get_Help());
    }

    TEST_METHOD(Parse_Help_Question)
    {
        CommandArgs args;
        Assert::IsFalse(args.get_Help());

        args.Parse(L"vswhere.exe -h");
        Assert::IsTrue(args.get_Help());
    }

    TEST_METHOD(Parse_Help_Long)
    {
        CommandArgs args;
        Assert::IsFalse(args.get_Help());

        args.Parse(L"vswhere.exe -help");
        Assert::IsTrue(args.get_Help());
    }

    TEST_METHOD(Parse_Unknown_Parameter)
    {
        CommandArgs args;

        Assert::ExpectException<win32_error>([&] { args.Parse(L"vswhere.exe -unknown"); });
    }

    TEST_METHOD(Parse_Unknown_Argument)
    {
        CommandArgs args;

        Assert::ExpectException<win32_error>([&] { args.Parse(L"vswhere.exe unknown"); });
    }
};
