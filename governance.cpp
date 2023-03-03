/*
    VoteCand
    Votes for a candidate
    Parameters:
        user: name of the user voting
        election_id: id of the election
        candidates: vector of candidates to vote for
        staked_asset: id of the staked asset
    Return value:
        None
    Example:
        votecand("testuser"_n, 1, {"testcand1"_n, "testcand2"_n}, 1)
    Notes:
        This function is called by the user.
        This function checks if the election exists.
        This function checks if the election is active.
        This function checks if the user is a member of the dao and if the membership started before the election.
        This function checks if the user has enough voting power to vote for all candidates.
        This function checks if the user has already voted for the election.
        This function checks if the candidates exist.
        This function checks if the candidates are not already elected.
        This function checks if the candidates are not already disqual
*/
void gangdao::votecand(name user, uint64_t election_id, vector<eosio::name> candidates, uint64_t staked_asset){
    require_auth(user);
    
    // check if election exists
    elections_table elections_tab(_self, _self.value);
    auto elections_itr = elections_tab.find(election_id);
    check (elections_itr != elections_tab.end(), "election does not exist");

    eosio::time_point_sec tps = eosio::current_time_point();
    uint64_t COUNT_SECONDS (tps.sec_since_epoch());

    // check if election is active
    check (elections_itr->end > COUNT_SECONDS, "election is not active");

    name dao_name = elections_itr->dao_name;

    // check if user is a member of the dao and if the membership started before the election
    membership_table members_table(_self, elections_itr->dao_name.value);
    auto members_itr = members_table.find(user.value);
    check (members_itr != members_table.end(), "user is not a member of the dao");
    check (members_itr->reg_time < elections_itr->start, "user is not a member of the dao at the time of the election");

    // get user voting power
    uint16_t voting_power = members_itr->votes;

    // divide voting power by number of candidates and round down
    uint16_t vote = voting_power / candidates.size();

    // modify the elections table
    elections_tab.modify(elections_itr, _self, [&](auto& row){
        row.votes_issued += voting_power;
    });

    // modify candidates table
    candidates_table candidates_tab(_self, elections_itr->dao_name.value);
    for (auto cand : candidates){
        auto votes_itr = candidates_tab.find(cand.value);
        if (votes_itr == candidates_tab.end()){
            candidates_tab.emplace(_self, [&](auto& row){
                row.cand_name = cand;
                row.votes = vote;
                row.election_id = election_id;
                row.dao_name = dao_name;
            });
        } else {
            candidates_tab.modify(votes_itr, _self, [&](auto& row){
                row.votes += vote;
            });
        }
    }

}

/*
    AddCandidate
    Adds a candidate to an election
    Parameters:
        cand: name of the candidate
        election_id: id of the election
        dao_name: name of the DAO
        asset_id: id of the staked asset
    Return value:
        None
    Example:
        addcand("testcand"_n, 1, "testdao"_n, 1)
    Notes:
        This function is called by the candidate.
        This function checks if the candidate is already registered for this election.
        This function adds the candidate to the candidates table.
*/
void gangdao::addcandidate(name cand, uint64_t election_id, name dao_name, uint64_t asset_id){
    require_auth(cand);
    
    // do bitwise operation to generate a uint128_t variable from cand.value and election_id 
    uint128_t candidate_election_id = (uint128_t(election_id) << 64) | cand.value;

    // check if candidate is already registered for this election by querying a candidates table secondary index
    candidates_table candidates_tab(_self, dao_name.value);
    auto by_special_id = candidates_tab.get_index<"bycandelect"_n>();
    auto candidate_itr = by_special_id.find(candidate_election_id);
    check (candidate_itr == by_special_id.end(), "candidate is already registered for this election");


    // add candidate to candidates table
    candidates_tab.emplace(_self, [&](auto& row){
        row.cand_name = cand;
        row.votes = 0;
        row.election_id = election_id;
        row.dao_name = dao_name;
    });
    
}


/*
    StartElect
    Starts an election
    Parameters:
        dao_name: name of the DAO
        end: end time of the election
        elected_n: number of candidates to be elected
    Return value:
        None
    Example:
        startelect("testdao"_n, 1000000000, 2)
    Notes:
        This function is called by the DAO.
        This function checks if the DAO exists.
        This function adds the election to the elections table.
*/
void gangdao::startelect(name dao_name, uint64_t end, uint8_t elected_n){
    require_auth(dao_name);

    // check if dao exists
    daos_index daos_table(_self, _self.value);
    auto daos_itr = daos_table.find(dao_name.value);
    check (daos_itr != daos_table.end(), "dao does not exist");

    eosio::time_point_sec tps = eosio::current_time_point();
    uint64_t COUNT_SECONDS (tps.sec_since_epoch());
    // add election to elections table
    elections_table elections_tab(_self, _self.value);
    elections_tab.emplace(_self, [&](auto& row){
        row.election_id = elections_tab.available_primary_key();
        row.dao_name = dao_name;
        row.start = COUNT_SECONDS;
        row.end = end;
        row.elected_n = elected_n;
        row.votes_issued = 0;
    });

    

}

/*
    FinishElect
    Finishes an election
    Parameters:
        dao_name: name of the DAO
        election_id: id of the election
    Return value:
        None
    Example:
        finishelect("testdao"_n, 1)
    Notes:
        This function is called by the DAO.
        This function checks if the election exists.
        This function checks if the election is over.
        This function gets the elected candidates and adds them to the elected table.
*/
void gangdao::finishelect(name dao_name, uint64_t election_id){
    require_auth(dao_name);

    // check if election exists
    elections_table elections_tab(_self, _self.value);
    auto elections_itr = elections_tab.find(election_id);
    check (elections_itr != elections_tab.end(), "election does not exist");

    // check if election is over
    eosio::time_point_sec tps = eosio::current_time_point();
    uint64_t COUNT_SECONDS (tps.sec_since_epoch());
    check (elections_itr->end < COUNT_SECONDS, "election is not over");

    // get elected candidates
    candidates_table candidates_tab(_self, dao_name.value);
    auto by_votes = candidates_tab.get_index<"byvotes"_n>();
    auto votes_itr = by_votes.begin();
    vector<electedcandidate> elected;
    for (int i = 0; i < elections_itr->elected_n; i++){
        electedcandidate ec;
        ec.candidate = votes_itr->cand_name;
        ec.votes = votes_itr->votes;

        elected.push_back(ec);
        votes_itr++;
    }

    // MODIFY CUSTODIANS TABLE
    custodians_table custodians_tab(_self, dao_name.value);
    auto custodians_itr = custodians_tab.begin();
    // iterate through custodians table
    // if custodian has dao_name and is not in elected vector, erase it
    // if custodian has dao_name and is in elected vector, modify it
    // if elected vector is empty during the while loop, stop it
    // if elected vector does not get empty during the while loop, add the remaining custodians to the table
    
    while (custodians_itr != custodians_tab.end()){
        // check if elected vector is empty
        if (elected.empty()){
            break;
        }

        if (custodians_itr-> dao_name == dao_name){
            for (uint8_t i = 0; i < elected.size(); i++){
                if (elected[i].candidate == custodians_itr->cust_name){
                    // modify custodians_itr to fit current election
                    custodians_tab.modify(custodians_itr, _self, [&](auto& row){
                        row.cust_end = COUNT_SECONDS + (3600 * 24 * 30);
                        row.total_votes = elected[i].votes;
                    });
                    // erase cand from elected vector
                    elected.erase(elected.begin() + i);
                    // go to next custodian_itr
                    custodians_itr++;
                } 
            }
        }
    }

    // if elected vector is not empty, add the remaining custodians to the table
    if (!elected.empty()){
        for (auto cand : elected){
            custodians_tab.emplace(_self, [&](auto& row){
                row.cust_name = cand.candidate;
                row.cust_end = COUNT_SECONDS + (3600 * 24 * 30);
                row.total_votes = cand.votes;
                row.dao_name = dao_name;
            });

        }

    }
    

}