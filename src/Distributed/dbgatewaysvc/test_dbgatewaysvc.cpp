#include "file_to_db.hpp"
#include "test_sql.hpp"
#include "test_dbg.hpp"
#include "test_dbg_perf.hpp"

int main(int, char **)
{
	//test_dbg_perf_main();
	//test_dbg_main();
	//test_sql_main();
	file_to_db_main();
	getchar();

	return 0;
}


