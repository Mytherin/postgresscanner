//===----------------------------------------------------------------------===//
//                         DuckDB
//
// storage/postgres_transaction.hpp
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include "duckdb/transaction/transaction.hpp"
#include "duckdb/common/case_insensitive_map.hpp"
#include "postgres_connection.hpp"

namespace duckdb {
class PostgresCatalog;
class PostgresSchemaEntry;
class PostgresTableEntry;

class PostgresTransaction : public Transaction {
public:
	PostgresTransaction(PostgresCatalog &postgres_catalog, TransactionManager &manager, ClientContext &context);
	~PostgresTransaction() override;

	void Start();
	void Commit();
	void Rollback();

	PostgresConnection &GetConnection();
	optional_ptr<CatalogEntry> GetCatalogEntry(CatalogType type, PostgresSchemaEntry &schema, const string &table_name);
	vector<reference<CatalogEntry>> GetEntries(CatalogType type, PostgresSchemaEntry &schema);
	void DropEntry(CatalogType type, const string &table_name, bool cascade);
	void ClearTableEntry(const string &table_name);

	static PostgresTransaction &Get(ClientContext &context, Catalog &catalog);

private:
	PostgresCatalog &postgres_catalog;
	PostgresConnection connection;
	case_insensitive_map_t<unique_ptr<CatalogEntry>> catalog_entries;
};

} // namespace duckdb