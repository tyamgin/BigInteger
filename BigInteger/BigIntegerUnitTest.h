
using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace MyTest
{
  TEST_CLASS(MyTests)
  {
  public:
      TEST_METHOD(MyTestMethod)
      {
          Assert::AreEqual(MyProject::Multiply(2,3), 6);
      }
  };
}