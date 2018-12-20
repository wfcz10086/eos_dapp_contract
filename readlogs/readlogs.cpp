#include <eosiolib/eosio.hpp>
#include <eosiolib/print.hpp>
#include <string> 
using namespace eosio;


class [[eosio::contract]] readlogs : public eosio::contract {

public:
  using contract::contract;
  readlogs(name receiver, name code,  datastream<const char*> ds): contract(receiver, code, ds) {}


  [[eosio::action]]
  void insert(name user, std::string url, std::string readname , uint64_t read_state ) {
    require_auth(user);
    readlogs_index reads(_code, _code.value);
    reads.emplace(user, [&]( auto& row ) {
       row.id =reads.available_primary_key(),
       row.user=user;
       row.url = url;
       row.readname = readname;
       row.read_state = 0;
       action_sum(row.id, "insert");
      });
    std::string msg = "区块链中您的记录为"+std::to_string(reads.available_primary_key())+"成功添加了url为"+url+"点阅读记录,阅读记录被记录为"+readname;
    message_notification(user, msg);
    action_sum(reads.available_primary_key(), "insert");

  }

 [[eosio::action]]
  void update(uint64_t id,uint64_t read_state ) {
    readlogs_index reads(_code, _code.value);
    auto it_1 = reads.find(id);
    eosio_assert(it_1 != reads.end(), "Record does not exist");
    require_auth(it_1->user);   
    if( it_1 == reads.end() ){
    reads.modify(it_1, it_1->user, [&]( auto& row ) {
       row.read_state = read_state;        
      });    
    }
    std::string msg = "成功更新了url为"+it_1->url+",阅读记录为"+it_1->readname+",id为"+std::to_string(id)+"的状态read_statue:"+std::to_string(read_state);
    message_notification(it_1->user, msg);
    action_sum(id, "update");


  }




 [[eosio::action]]
  void erase(uint64_t id) {
    readlogs_index reads(_code, _code.value);
    auto iterator = reads.find(id);
    eosio_assert(iterator != reads.end(), "Record does not exist");
    require_auth(iterator->user);   
    reads.erase(iterator);
    std::string msg = "成功删除了id为"+std::to_string(id)+"阅读记录";
    message_notification(iterator->user, msg);
    action_sum(id, "erase");

  }


[[eosio::action]]
  void notify(name user, std::string msg) {
    require_auth(get_self());
    require_recipient(user);
  }

private:
  void message_notification(name account, std::string msg) {
    action(permission_level{get_self(),"active"_n},get_self(),"notify"_n,std::make_tuple(account, name{account}.to_string() + msg)).send();
  };

void action_sum(uint64_t id, std::string type) {
    
    action counter = action(
      permission_level{get_self(),"active"_n},
      "readlogssum"_n,
      "count"_n,
      std::make_tuple(id, type)
    );

    counter.send();
  }

  
  struct [[eosio::table]] logs {
    uint64_t id;
    name user;
    std::string url;
    std::string readname;
    uint64_t read_state;
    uint64_t primary_key() const { return id; }
    uint64_t primary_key_1() const { return user.value; }

  };
  typedef eosio::multi_index<"logstable"_n, logs, indexed_by<"byname"_n, const_mem_fun<logs, uint64_t, &logs::primary_key_1>>> readlogs_index;

};
EOSIO_DISPATCH( readlogs, (insert)(update)(erase)(notify))
