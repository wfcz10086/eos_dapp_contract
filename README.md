# eos_dapp_contract
  注意版本：合约代码 1.3.2 和1.4
  
  有很大区别
# command
   cleos create account eosio bob EOS62NHgkUyMUkSV7NahnLxvxnMi43hvwRpe8KhK9V9ML6YqTQbn1
   cleos create account eosio alice EOS7q4ndM2v7EcYmR6P1KvbUj6sQpn1oR6saDqVy59qaYR5zutqmr
   cleos create account eosio readlogssum EOS7q4ndM2v7EcYmR6P1KvbUj6sQpn1oR6saDqVy59qaYR5zutqmr
   cleos create account eosio readlogs EOS7q4ndM2v7EcYmR6P1KvbUj6sQpn1oR6saDqVy59qaYR5zutqmr

   cleos push action readlogs insert '["alice","http://baidu.com/1","baidu1",1]' -p alice@active
   cleos push action readlogs erase [0] -p alice@active
   cleos push action readlogs erase [0] -p alice@active
   ./MakeContract readlogs
   ./MakeContract readlogssum
   cleos set contract readlogssum  readlogssum
   cleos set contract readlogs  readlogs
   cleos push action readlogs insert '["alice","http://baidu.com/1","baidu1",1]' -p alice@active
   cleos push action readlogs insert '["alice","http://baidu.com/1","baidu1",1]' -p alice@active
