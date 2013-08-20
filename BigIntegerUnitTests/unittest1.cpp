//#include "stdafx.h"
//
//using namespace Microsoft::VisualStudio::CppUnitTestFramework;
//
//namespace BigIntegerUnitTests
//{		
//	TEST_CLASS(UnitTest1)
//	{
//	public:
//		
//		TEST_METHOD(TestMethod)
//		{
//			Assert::AreEqual(1, 1);
//			Assert::AreEqual(0.0, 0.0);
//		}
//
//		TEST_METHOD(Equal)
//		{
//			Assert::IsTrue(BigInteger::ONE == BigInteger("1"));
//			Assert::IsTrue(BigInteger(123) == BigInteger("123"));
//		}
//
//		TEST_METHOD(MultiplicationSimple)
//		{
//			Assert::IsTrue(BigInteger(123) * BigInteger(0) == BigInteger::ZERO);
//			Assert::IsTrue(BigInteger(2) * 2 == BigInteger(4));
//		}
//
//		TEST_METHOD(DivideSimple)
//		{
//			Assert::IsTrue(BigInteger(123) / BigInteger(1) == BigInteger(123));
//			Assert::IsTrue(BigInteger::ZERO / BigInteger("234234234234234234234235235345346") == BigInteger::ZERO);
//		}
//	};
//}