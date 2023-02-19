void gangdao::stake(const name& from,
        const name& to,
        vector<uint64_t> asset_ids,
        const string memo){

    require_auth(from);
    check(to == get_self(), "This action is only valid if sent to the contract");
    check(asset_ids.size() > 0, "No assets were sent");
    check(asset_ids.size() < 2, "Cannot stake more than one membership");
    check(memo.size() > 0, "No memo was sent");
    check(memo.size() < 13, "Memo is too long");
    check(is_account(eosio::name(memo)), "Memo is not a valid account name");
    check(memo != "gangdao11111", "Cannot stake to gangdao11111");
    check(memo != "gangdao11112", "Cannot stake to gangdao11112");
    check(memo != "gangdao11113", "Cannot stake to gangdao11113");


    //define a function for both queries
    // find asset template id by querying atomicassets
    atomicassets::assets_t _assets = atomicassets::assets_t(ATOMIC, get_self().value);
    auto assets_it = _assets.find(asset_ids[0]);
    uint32_t this_asset_template_id = assets_it-> template_id;

    membership_config_index membership_config_table(get_self(), get_self().value);
    auto membership_config_it = membership_config_table.find(this_asset_template_id);
    check(membership_config_it != membership_config_table.end(), "This asset is not stakeable");
    check(membership_config_it-> dao_name == eosio::name(memo), "This asset is stakeable in other DAO");
    uint32_t membership_shares = membership_config_it-> vote_weight;
    uint32_t membership_votes = membership_config_it-> votes;

    // check if asset is already staked
    membership_index membership_table(get_self(), get_self().value);
    auto membership_it = membership_table.find(asset_ids[0]);
    check(membership_it == membership_table.end(), "This asset is already staked");

    // check if user is already a member of the DAO
    members_index members_table(get_self(), eosio::name(memo).value);
    auto members_it = members_table.find(from.value);
    check(members_it == members_table.end(), "User is already a member of this DAO");

    // add asset to the membership table
    membership_table.emplace(get_self(), [&](auto& row){
        row.asset_id = asset_ids[0];
        row.user = from;
        row.dao_name = eosio::name(memo);
        row.shares = membership_shares;
        row.votes = membership_votes;
        row.registered = eosio::current_time_point();
        row.last_p_claim = eosio::current_time_point();
    });

    // update total shares and votes
    totalweights_index totalweights(get_self(), get_self().value);
    auto totals_it = totalweights.find(eosio::name(memo).value);
    if(totals_it != totalweights.end()){
        totalweights.modify(totals_it, get_self(), [&](auto& row){
            row.total_shares += membership_shares;
            row.total_votes += membership_votes;
        });
    } else {
        totalweights.emplace(get_self(), [&](auto& row){
            row.dao_name = eosio::name(memo);
            row.total_shares = membership_shares;
            row.total_votes = membership_votes;
        });
    }
    
    }   

void gangdao::unstake(name user, uint64_t asset_id){

    require_auth(user);

    membership_index membership_table(get_self(), get_self().value);
    auto membership_it = membership_table.find(asset_id);
    check(membership_it != membership_table.end(), "This asset_id was not found");
    check(membership_it-> user == user, "One must own the asset to unstake it");

    // prepare the transfer
    vector <uint64_t> assets_to_transfer;
    assets_to_transfer.push_back(asset_id);
    std::string memo = "Unstaked membership";

    // transfer the asset back to the user
    nonvectoratomictransfer transfer_arguments;
    transfer_arguments.from = get_self();
    transfer_arguments.to = user;
    transfer_arguments.asset_id = assets_to_transfer;
    transfer_arguments.memo = memo;
    action send_asset_to_user = action(
        permission_level(get_self(), "active"_n),
        "atomicassets"_n,
        "transfer"_n,
        std::move(transfer_arguments)
    ); send_asset_to_user.send();

    // remove the asset from the membership table
    membership_table.erase(membership_it);

    // update total shares and votes
    totalweights_index totalweights(get_self(), get_self().value);
    auto totals_it = totalweights.find(membership_it-> dao_name.value);
    if(totals_it != totalweights.end()){
        totalweights.modify(totals_it, get_self(), [&](auto& row){
            row.total_shares -= membership_it-> shares;
            row.total_votes -= membership_it-> votes;
        });
    } else {
        totalweights.emplace(get_self(), [&](auto& row){
            row.dao_name = membership_it-> dao_name;
            row.total_shares = 0;
            row.total_votes = 0;
        });
    }

};
