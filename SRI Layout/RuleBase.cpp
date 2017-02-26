#include "RuleBase.hpp"
#include <iostream>
#include "Utility.hpp"
#include "SRI.hpp"

using namespace std;
using namespace utility;

void RuleBase::DropRule (string name)
{
    rules.erase(name);
}

void RuleBase::AddRule (string name, Subrule info)
{
    auto ruleEntry = rules.find (name);
    
    if ( ruleEntry == rules.end() )
    {
        rules[name] = vector<Subrule> {info};
    }
    else
    {
        rules[name].push_back(info);
    }
}

vector<vector<string>> RuleBase::GetResultSet(string name, vector<string> params)
{
    auto entry = rules.find(name);
    
    if ( entry == rules.end() )
    {
        vector<vector<string>> empty;
        return empty;
    }
    
    vector<vector<string>> results;
    
    vector<Subrule> subrules = entry->second;
    
    for (auto subrule : subrules)
    {
        if (subrule.parameters.size() != params.size())
            continue;
        
        vector<vector<string>> subruleResults;
        
        if (subrule.isAnd)
        {
            subruleResults = GetResultsAND(subrule, name, params);
        }
        else
        {
            subruleResults = GetResultsOR(subrule, name, params);
        }
        
        results.insert(end(results), begin(subruleResults), end(subruleResults));
    }
    
    return results;
}

vector<vector<string>> RuleBase::GetResultsOR(Subrule subrule, string name, vector<string> params)
{
    vector<vector<string>> results;
    
    for (auto clause : subrule.clauses)
    {
        vector<string> inputs;
        
        for (auto i : clause.parameters)
        {
            int index = FindIndexOf(subrule.parameters, i);
            
            if(index == -1)
                inputs.push_back(i);
            else
                inputs.push_back(params[index]);
        }
        
        vector<vector<string>> clauseSet = engine->GetSet(clause.name, inputs);
        results.insert(end(results), begin(clauseSet), end(clauseSet));
    }
    
    return results;
}

vector<vector<string>> RuleBase::GetResultsAND(Subrule subrule, string name, vector<string> params)
{
    int currentParamIndex = 0;
    unordered_map<string, int> paramToIndex;
    vector<vector<string>> candidates;
    
    // FOR EACH CLAUSE
    
    for (auto clause : subrule.clauses)
    {
        // ============================================
        // Get the possible inputs for the clause.
        // ============================================
        
        vector<vector<string>> clauseInputs;

        // FOR EACH PARAMETER
        
        for (auto param : clause.parameters)
        {
            vector<string> parameterInputs;
            
            // IF PARAMETER IS NOT A VARIABLE, INPUT IT AS IS
            
            if (!IsVariable(param))
            {
                parameterInputs.push_back(param);
                clauseInputs.push_back(parameterInputs);
                continue;
            }
            
            // IF PARAMETER HAS NOT YET BEEN USED IN PREV. CLAUSE, INPUT IT AS IS
            
            auto entry = paramToIndex.find(param);
            if ( entry == paramToIndex.end() )
            {
                parameterInputs.push_back(param);
                clauseInputs.push_back(parameterInputs);
                continue;
            }
            
            // IF PARAMETER HAS BEEN USED, USE THE RESULTS AS INPUT
            
            int paramIndex = entry->second;
            
            for (auto result : candidates)
                parameterInputs.push_back(result[paramIndex]);
            
            clauseInputs.push_back(parameterInputs);
        }
        
        // =============================================================
        // Based on all possible inputs for the clause, get all results.
        // =============================================================
        
        vector<vector<string>> clausePermutation = PermutateVector(clauseInputs);
        
        vector<vector<string>> clauseResults;
        for (auto i : clausePermutation)
        {
            auto temp = engine->GetSet(clause.name, i);
            clauseResults.insert(end(clauseResults), begin(temp), end(temp));
        }
        
        // ================================================================
        // If the clause has no possible results, the subrule has none too.
        // ================================================================
        
        if (clauseResults.size() == 0)
        {
            vector<vector<string>> empty;
            return empty;
        }
        
        // =========================================================
        // Delete all duplicate parameters in results - ex. ($X,$X).
        // =========================================================
        
        vector<string> usedClauseParameters;
        
        for (int i = 0; i < clause.parameters.size(); ++i)
        {
            string param = clause.parameters[i];
            
            if (FindIndexOf(usedClauseParameters, param) == -1)
            {
                usedClauseParameters.push_back(param);
                continue;
            }
            
            clause.parameters.erase(clause.parameters.begin() + i);
            for (auto & result : clauseResults)
            {
                result.erase(result.begin() + i);
            }
            
            i--;
        }
        
        // ======================================================================
        // Map each paramter to a spot in the vector and vice-versa.
        //    The $Z parameter in the clause result might be at a different index
        //    than the $Z parameter in the candidate results.
        // ======================================================================
        
        unordered_map<int, int> candidateToClauseIndex;
        
        // FOR EACH PARAMETER
        
        bool allNewParameters = true;
        
        for (int i=0; i<clause.parameters.size(); ++i)
        {
            string clauseParam = clause.parameters[i];
            
            // IF PARAMETER HAS NO ASSIGNED SPOT IN VECTOR,
                // GIVE PARAMETER ASSIGNED SPORT IN VECTOR AND LINK VECTOR INDEX TO CLAUSE INDEX
            
            auto entry = paramToIndex.find(clauseParam);
            if ( entry == paramToIndex.end() )
            {
                paramToIndex[clauseParam] = currentParamIndex;
                candidateToClauseIndex[currentParamIndex] = i;
                currentParamIndex++;
                continue;
            }
            
            // IF PARAMETER ALREADY HAS ASSIGNED SPOT IN VECTOR FROM PREV CLAUSE
                // LINK ASSIGNED VECTOR INDEX TO CLAUSE INDEX
            
            int resultIndex = entry->second;
            candidateToClauseIndex[resultIndex] = i;
            allNewParameters = false;
        }
        
        // =====================================================================================
        // Handle if the clause and candidate results do not have matching parameters.
        //   This happens on the first clause and on clauses with only new paramters.
        //   EX. Clause1($X,$Y) Clause2($Y,$Z) Clause3($Q,$R)
        //     [Clause1 and Clause3 have all new paramters]
        // =====================================================================================
        
        // IF CLAUSE CONTAINS ONLY NEW PARAMETERS
        if (allNewParameters)
        {
            int candidateNumber = (int) candidates.size();
            
            // IF NO EXISTING CANDIDATES EXIST, SET THE CLAUSE RESULTS AS THE NEW CANDIDATES
            
            if (candidateNumber == 0)
            {
                candidates = clauseResults;
                continue;
            }
            
            // IF CANDIDATES EXIST, PERMUTATE ALL EXISTING CANDIDATES WITH CLAUSE RESULTS
            
            else
            {
                vector<vector<string>> newResults;
                
                for (auto i : candidates)
                {
                    for (auto k : clauseResults)
                    {
                        vector<string> newResult;
                        newResult.reserve( i.size() + k.size());
                        newResult.insert(newResult.end(), i.begin(), i.end());
                        newResult.insert(newResult.end(), k.begin(), k.end());
                        
                        newResults.push_back(newResult);
                    }
                }
                
                candidates = newResults;
                
                continue;
            }
        }
        
        // ====================================================================
        // Handle if the clause and candidate results have matching parameters.
        // For each candidate, go through each new result:
        //   If the candidate does not match any new clause result, delete it.
        //   For each candidate does match a new clause result,
        //                             permutate it with all the new parameters.
        // ====================================================================
        
        int numberOfCandidates = (int) candidates.size();
        
        // FOR EACH CANDIDATE RESULT
        
        for (int candidateIndex = 0; candidateIndex < numberOfCandidates; ++candidateIndex)
        {
            auto candidate = candidates[candidateIndex];
            int candidateSize = (int) candidate.size();
            
            bool hasAnyMatch = false;

            // FOR EACH NEW RESULT
            
            for (auto clauseResult : clauseResults)
            {
                bool resultIsMatch = true;
                
                // FOR EACH PARAMETER IN THE CANDIDATE RESULT
                
                for (int i=0; i<candidateSize; ++i)
                {
                    // IF THE PARAMETER IS NOT PART OF THE NEW RESULT, IGNORE IT
                    
                    auto entry = candidateToClauseIndex.find(i);
                    if ( entry == candidateToClauseIndex.end() )
                    {
                        continue;
                    }
                    int index = entry->second;
                    
                    // IF THE VALUE OF THE PARAMETER IN THE CANDIDATE AND NEW RESULT ARE NOT THE SAME,
                        // THE CANDIDATE RESULT AND TEH NEW RESULT ARE NOT A MATCH
                    
                    if (candidate[i] != clauseResult[index])
                    {
                        resultIsMatch = false;
                        break;
                    }
                }
                
                // IF THE CANDIDATE RESULT IS A MATCH WITH THE NEW RESULT
                
                if (resultIsMatch)
                {
                    hasAnyMatch = true;
                    
                    // IF THIS IS THE CANDIDATE RESULT'S FIRST NEW MATCH.
                        // APPEND THE NEW VALUES
                    
                    if (candidate.size() < currentParamIndex)
                    {
                        while (candidate.size() < currentParamIndex)
                        {
                            int clauseParameterIndex = (int) candidateToClauseIndex[candidate.size()];
                            string newParameter = clauseResult[clauseParameterIndex];
                            candidate.push_back(newParameter);
                            candidates[candidateIndex] = candidate;
                        }
                    }
                    else
                    {
                        // ELSE DUPLICATE THIS VECTOR
                        
                        vector<string> newCandidate;
                        
                        for (int index = 0; index < candidateSize; ++index)
                        {
                            newCandidate.push_back(candidate[index]);
                        }
                        
                        // APPEND THE NEW VALUES TO THE NEW VECTOR
                        
                        while (newCandidate.size() < currentParamIndex)
                        {    
                            int clauseParameterIndex = (int) candidateToClauseIndex[newCandidate.size()];
                            string newParameter = clauseResult[clauseParameterIndex];
                            newCandidate.push_back(newParameter);
                        }

                        candidates.push_back(newCandidate);
                    }
                }
            }
            
            // IF NONE OF THE NEW RESULTS MATCH THE CANDIDATE RESULT,
                // DELETE THE CANDIDATE RESULT
            
            if (!hasAnyMatch)
            {
                candidates.erase(candidates.begin() + candidateIndex);
                numberOfCandidates--;
                candidateIndex--;
                continue;
            }
        }
    }
    
    // =========================================================
    // If there are no candidates left, return candidates
    // =========================================================

    if (candidates.size() == 0)
    {
        return candidates;
    }
    
    // =========================================================
    // Return only the paramters asked for my the subrule.
    // =========================================================
    
    vector<vector<string>> results(candidates.size());
    
    for (auto param : subrule.parameters)
    {
        int paramIndex = paramToIndex[param];
        
        for (int i=0; i < candidates.size(); i++)
        {
            results[i].push_back(candidates[i][paramIndex]);
        }
    }
    
    return results;
}

void RuleBase::Export(ostream& file)
{
    for (auto rule : rules)
    {
        file << "RULE " << rule.first;
        
        for (auto subrule : rule.second)
        {
            
            file << "(";
            // NOTE: could clean this up later to use an iterator
            for (int i=0; i<subrule.parameters.size(); i++)
            {
                if( i != 0 ){ file << ","; }
                file << subrule.parameters[i];
            }
            file << "):- ";
            
            file << ((subrule.isAnd) ? "AND" : "OR");
            
            for (auto clause : subrule.clauses)
            {
                file << " " << clause.name << "(";
                for (int i=0; i<clause.parameters.size(); i++)
                {
                    if( i != 0 )
                       file << ",";
                    file << clause.parameters[i];
                }
                file << ")";
            }
            file << endl;
            
        }
    }
    
}
