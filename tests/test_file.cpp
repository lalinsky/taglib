/***************************************************************************
    copyright           : (C) 2014 by Lukas Lalinsky
    email               : lukas@oxygene.sk
 ***************************************************************************/

/***************************************************************************
 *   This library is free software; you can redistribute it and/or modify  *
 *   it  under the terms of the GNU Lesser General Public License version  *
 *   2.1 as published by the Free Software Foundation.                     *
 *                                                                         *
 *   This library is distributed in the hope that it will be useful, but   *
 *   WITHOUT ANY WARRANTY; without even the implied warranty of            *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU     *
 *   Lesser General Public License for more details.                       *
 *                                                                         *
 *   You should have received a copy of the GNU Lesser General Public      *
 *   License along with this library; if not, write to the Free Software   *
 *   Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,            *
 *   MA  02110-1301  USA                                                   *
 ***************************************************************************/

#include <tfile.h>
#include <cppunit/extensions/HelperMacros.h>
#include "utils.h"

using namespace TagLib;

// File subclass that gives tests access to filesystem operations
class PlainFile : public File {
public:
  PlainFile(FileName name) : File(name) { }
  Tag *tag() const { return NULL; }
  AudioProperties *audioProperties() const { return NULL; }
  bool save(){ return false; }
  void truncate(long length) { File::truncate(length); }
};

class TestFile : public CppUnit::TestFixture
{
  CPPUNIT_TEST_SUITE(TestFile);
  CPPUNIT_TEST(testFindInSmallFile);
  CPPUNIT_TEST(testRFindInSmallFile);
  CPPUNIT_TEST_SUITE_END();

public:

  void testFindInSmallFile()
  {
    ScopedFileCopy copy("empty.ogg", "ogg");
    std::string name = copy.fileName();
    {
      PlainFile file(name.c_str());
      file.seek(0);
      file.writeBlock(ByteVector("0123456239", 10));
      file.truncate(10);
    }
    {
      PlainFile file(name.c_str());
      CPPUNIT_ASSERT_EQUAL(10l, file.length());
      CPPUNIT_ASSERT_EQUAL(2l, file.find(ByteVector("23", 2)));
      CPPUNIT_ASSERT_EQUAL(2l, file.find(ByteVector("23", 2), 2));
      CPPUNIT_ASSERT_EQUAL(7l, file.find(ByteVector("23", 2), 3));
    }
  }

  void testRFindInSmallFile()
  {
    ScopedFileCopy copy("empty.ogg", "ogg");
    std::string name = copy.fileName();
    {
      PlainFile file(name.c_str());
      file.seek(0);
      file.writeBlock(ByteVector("0123456239", 10));
      file.truncate(10);
    }
    {
      PlainFile file(name.c_str());
      CPPUNIT_ASSERT_EQUAL(10l, file.length());
      CPPUNIT_ASSERT_EQUAL(7l, file.rfind(ByteVector("23", 2)));
      CPPUNIT_ASSERT_EQUAL(7l, file.rfind(ByteVector("23", 2), 7));
      CPPUNIT_ASSERT_EQUAL(2l, file.rfind(ByteVector("23", 2), 6));
    }
  }

};

CPPUNIT_TEST_SUITE_REGISTRATION(TestFile);
