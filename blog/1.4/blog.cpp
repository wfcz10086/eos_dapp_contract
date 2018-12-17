#include "blog.hpp"

using namespace eosio;
using std::string;

class blog : public contract
{
  using contract::contract;

public:

  blog(eosio::name receiver, eosio::name code,  eosio::datastream<const char*> ds): contract(receiver, code, ds) {}

  [[eosio::action]]

  void createpost(const eosio::name author, const string &title, const string &content, const string &tag)
  {

    require_auth(author);
    post_index posts(_code, _code.value);



    posts.emplace(author, [&](auto &post) {
      post.pkey = posts.available_primary_key();
      post.author = author;
      post.title = title;
      post.content = content;
      post.tag = tag;
      post.likes = 0;
    });
  }

  [[eosio::action]]

  void deletepost(const uint64_t pkey)
  {
    post_index posts(_code, _code.value);

    auto iterator = posts.find(pkey);
    eosio_assert(iterator != posts.end(), "Post for pkey could not be found");

    require_auth(iterator->author);

    posts.erase(iterator);
  }

  [[eosio::action]]


  void editpost(const uint64_t pkey, const string &title, const string &content, const string &tag)
  {
    post_index posts(_code, _code.value);

    auto iterator = posts.find(pkey);
    eosio_assert(iterator != posts.end(), "Post for pkey could not be found");

    require_auth(iterator->author);

    posts.modify(iterator, iterator->author, [&](auto &post) {
      post.title = title;
      post.content = content;
      post.tag = tag;
    });
  }

[[eosio::action]]

  void likepost(const uint64_t pkey)
  {

    post_index posts(_code, _code.value);

    auto iterator = posts.find(pkey);
    eosio_assert(iterator != posts.end(), "Post for pkey not found");

    posts.modify(iterator, iterator->author, [&](auto &post) {
      print("Liking: ",post.title.c_str(), " By: ", post.author, "\n");
      post.likes++;
    });
  }

private:

  struct [[eosio::table]] post
  {
    uint64_t pkey;
    name author;
    std::string title;
    std::string content;
    std::string tag;
    uint64_t likes = 0;

    auto primary_key() const { return pkey; }

    uint64_t get_author() const { return author.value; }
    std::string get_tag() const { return tag; }

    EOSLIB_SERIALIZE(post, (pkey)(author)(title)(content)(tag)(likes))
  };

  typedef eosio::multi_index<"post"_n, post,indexed_by<"byauthor"_n, const_mem_fun<post, uint64_t, &post::get_author>>> post_index;



};

EOSIO_DISPATCH(blog, (createpost)(deletepost)(likepost)(editpost))