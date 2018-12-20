#include <eosiolib/eosio.hpp>
#include <eosiolib/print.hpp>

using namespace eosio;

class [[eosio::contract]] readlogssum : public eosio::contract {
  public:
    using contract::contract;

    readlogssum(name receiver, name code,  datastream<const char*> ds): contract(receiver, code, ds) {}

    [[eosio::action]]
    void count(uint64_t id, std::string type) {

      require_auth( name("readlogs"));

      count_index counts(name(_code), _code.value);
      auto it_1 = counts.find(id);

      
      if (it_1 == counts.end()) {
      eosio::print("正在插入数据");


        counts.emplace("readlogs"_n, [&]( auto& row ) {
          row.id = id;
          row.insertd = (type == "insert") ? 1 : 0;
          row.modified = (type == "update") ? 1 : 0;
          row.erased = (type == "erase") ? 1 : 0;

        });
      }
      else {
        eosio::print("正在更新数据");
        counts.modify(it_1, "readlogs"_n, [&]( auto& row ) {
          if(type == "insert") { row.insertd += 1; }
          if(type == "update") { row.modified += 1; }
          if(type == "erase") { row.erased += 1; }
        });
      }
    }

  private:
    struct [[eosio::table]] counter {
      uint64_t id;
      uint64_t insertd;
      uint64_t modified;
      uint64_t erased;
      uint64_t primary_key() const { return id; }
    };

    using count_index = eosio::multi_index<"counts"_n, counter>;
};

EOSIO_DISPATCH( readlogssum, (count));
