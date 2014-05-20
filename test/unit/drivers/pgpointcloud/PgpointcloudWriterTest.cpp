/******************************************************************************
* Copyright (c) 2014, Pete Gadomski (pete.gadomski@gmail.com)
*
* All rights reserved.
*
* Redistribution and use in source and binary forms, with or without
* modification, are permitted provided that the following
* conditions are met:
*
*     * Redistributions of source code must retain the above copyright
*       notice, this list of conditions and the following disclaimer.
*     * Redistributions in binary form must reproduce the above copyright
*       notice, this list of conditions and the following disclaimer in
*       the documentation and/or other materials provided
*       with the distribution.
*     * Neither the name of Hobu, Inc. or Flaxen Geo Consulting nor the
*       names of its contributors may be used to endorse or promote
*       products derived from this software without specific prior
*       written permission.
*
* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
* "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
* LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
* FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
* COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
* INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
* BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
* OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
* AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
* OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT
* OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY
* OF SUCH DAMAGE.
****************************************************************************/

#include <boost/test/unit_test.hpp>

#include <pdal/drivers/las/Reader.hpp>
#include <pdal/drivers/pgpointcloud/Writer.hpp>

#include "Support.hpp"
#include "drivers/pgpointcloud/Support.hpp"


pdal::Options getWriterOptions()
{
    pdal::Options options;

    options.add(pdal::Option("connection", pdal::drivers::pgpointcloud::testDbTempConn));
    options.add(pdal::Option("table", "pdal_test_table"));
    options.add(pdal::Option("srid", "4326"));
    options.add(pdal::Option("capacity", "10000"));
    
    return options;
}

struct PgpointcloudWriterTestFixture
{
    PgpointcloudWriterTestFixture()
        : m_masterConnection(
                pdal::drivers::pgpointcloud::pg_connect(
                    pdal::drivers::pgpointcloud::testDbConn))
        , m_testConnection(NULL)
    {
        // Silence those pesky notices
        executeOnMasterDb("SET client_min_messages TO WARNING");

        dropTestDb();

        std::stringstream createDbSql;
        createDbSql << "CREATE DATABASE " <<
            pdal::drivers::pgpointcloud::testDbTempname << " TEMPLATE template0";
        executeOnMasterDb(createDbSql.str());
        m_testConnection = pdal::drivers::pgpointcloud::pg_connect(
                pdal::drivers::pgpointcloud::testDbTempConn);

        executeOnTestDb("CREATE EXTENSION pointcloud");
    }

    void executeOnTestDb(const std::string& sql)
    {
        pdal::drivers::pgpointcloud::pg_execute(m_testConnection, sql);
    }

    ~PgpointcloudWriterTestFixture()
    {
        if (m_testConnection)
        {
            PQfinish(m_testConnection);
        }
        dropTestDb();
        if (m_masterConnection)
        {
            PQfinish(m_masterConnection);
        }
    }

private:

    void executeOnMasterDb(const std::string& sql)
    {
        pdal::drivers::pgpointcloud::pg_execute(m_masterConnection, sql);
    }

    void execute(PGconn* connection, const std::string& sql)
    {
        if (connection)
        {
            pdal::drivers::pgpointcloud::pg_execute(connection, sql);
        }
        else
        {
            throw std::runtime_error("Not connected to database for testing");
        }
    }

    void dropTestDb()
    {
        std::stringstream dropDbSql;
        dropDbSql << "DROP DATABASE IF EXISTS " << pdal::drivers::pgpointcloud::testDbTempname;
        executeOnMasterDb(dropDbSql.str());
    }

    PGconn* m_masterConnection;
    PGconn* m_testConnection;

};

BOOST_FIXTURE_TEST_SUITE(PgpointcloudWriterTest, PgpointcloudWriterTestFixture)


BOOST_AUTO_TEST_CASE(testWrite)
{
    pdal::drivers::las::Reader reader(Support::datapath("1.2-with-color.las"));
    pdal::drivers::pgpointcloud::Writer writer(reader, getWriterOptions());

    writer.initialize();

    boost::uint64_t numPointsWritten = writer.write(0);
    BOOST_CHECK_EQUAL(numPointsWritten, 1065);
}


BOOST_AUTO_TEST_CASE(testNoPointcloudExtension)
{
    executeOnTestDb("DROP EXTENSION pointcloud");

    pdal::drivers::las::Reader reader(Support::datapath("1.2-with-color.las"));
    pdal::drivers::pgpointcloud::Writer writer(reader, getWriterOptions());

    writer.initialize();

    BOOST_CHECK_THROW(writer.write(0), pdal::pdal_error);
}


BOOST_AUTO_TEST_SUITE_END()
