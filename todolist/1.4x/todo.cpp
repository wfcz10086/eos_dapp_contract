#include <eosiolib/eosio.hpp>
#include <eosiolib/print.hpp>

class todo_contract : public eosio::contract {
  public:
    using contract::contract;



  todo_contract(eosio::name receiver, eosio::name code,  eosio::datastream<const char*> ds): contract(receiver, code, ds) {}

  [[eosio::action]]

    void create(eosio::name author, const uint32_t id, const std::string& description) {
      todo_table todos(_self, _code.value);
      todos.emplace(author, [&](auto& new_todo) {
        new_todo.id  = id;
        new_todo.description = description;
        new_todo.completed = 0;
      });

      eosio::print("todo#", id, " created");
    }

    [[eosio::action]]

    void destroy(eosio::name author, const uint32_t id) {
      todo_table todos(_self, _code.value);

      auto todo_lookup = todos.find(id);
      todos.erase(todo_lookup);

      eosio::print("todo#", id, " destroyed");
    }
    [[eosio::action]]

    // @abi action
    void complete(eosio::name author, const uint32_t id) {
      todo_table todos(_self, _code.value);

      auto todo_lookup = todos.find(id);
      eosio_assert(todo_lookup != todos.end(), "Todo does not exist");

      todos.modify(todo_lookup, author, [&](auto& modifiable_todo) {
        modifiable_todo.completed = 1;
      });

      eosio::print("todo#", id, " marked as complete");
    }

  private:
    // @abi table todos i64
    struct [[eosio::table]] todo {
      uint64_t id;
      std::string description;
      uint64_t completed;

      uint64_t primary_key() const { return id; }
      EOSLIB_SERIALIZE(todo, (id)(description)(completed))
    };

    typedef eosio::multi_index<"todos"_n, todo> todo_table;
};

EOSIO_DISPATCH(todo_contract, (create)(complete)(destroy))