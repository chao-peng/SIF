#include <fstream>
#include <iostream>
#include <sstream>

#include <nlohmann/json.hpp>
#include <libSif/ASTAnalyser.hpp>
#include <libUtils/Constants.hpp>
#include <libUtils/Utils.hpp>
#include <cxxopts.hpp>

std::string global_solname1, global_solname2;

void report_issue(const std::string& name, const std::string& diff1, const std::string& diff2) {
    if (diff1 != diff2) {
        std::cout << name << " differs.\n\t"
              << "In [" << global_solname1 << "]\t" << diff1 << "\n\t"
              << "In [" << global_solname2 << "]\t" << diff2 << "\n";
    }
}

void report_issue(const std::string& name, const bool& diff1, const bool& diff2) {
    std::cout << name << " differs.\n\t"
              << "In [" << global_solname1 << "]\t" << (diff1? "True": "False") << "\n\t"
              << "In [" << global_solname2 << "]\t" << (diff2? "True": "False") << "\n";
}

int main(int argc, char** argv){
    std::string ast_file_name, ast_json_file_name, output_file_name, visitor_arg;
    std::string ast_file_name2, ast_json_file_name2;
    bool silent_mode = false;
    try{
        cxxopts::Options options("Sif", "Solidity Source Code Instrumentation Framework");
        options.add_options()
            ("a,ast", "AST in plain text file name", cxxopts::value<std::string>())
            ("j,json", "AST in compact JSON format file name", cxxopts::value<std::string>())
            ("b,bast", "AST in plain text file name", cxxopts::value<std::string>())
            ("k,bjson", "AST in compact JSON format file name", cxxopts::value<std::string>())
            //("b,abi", "ABI file name", cxxopts::value<std::string>())
            ("o,output","Output file name", cxxopts::value<std::string>())
            ("h,help", "Print help message")
            ("v,visitor_arg", "AST visitor arguments", cxxopts::value<std::string>())
            ("s,silent", "Silent mode", cxxopts::value<bool>());
        auto result = options.parse(argc, argv);
        if (result.count("help")) {
            std::cout << options.help({"", "Group"}) << std::endl;
            exit(Sif::ErrorCode::SUCCESS);
        }
        if (result.count("ast")) {
            ast_file_name = result["ast"].as<std::string>();
        } else {
            std::cout << "Plain-text AST file not provided\n";
            exit(Sif::ErrorCode::TEXT_AST_FILE_NOT_PROVIDED);
        }
        if (result.count("json")) {
            ast_json_file_name = result["json"].as<std::string>();
        } else {
            std::cout << "JSON AST file not provided\n";
            exit(Sif::ErrorCode::JSON_AST_FILE_NOT_PROVIDED);
        }
        if (result.count("bast")) {
            ast_file_name2 = result["bast"].as<std::string>();
        } else {
            std::cout << "Plain-text AST file not provided\n";
            exit(Sif::ErrorCode::TEXT_AST_FILE_NOT_PROVIDED);
        }
        if (result.count("bjson")) {
            ast_json_file_name2 = result["bjson"].as<std::string>();
        } else {
            std::cout << "JSON AST file not provided\n";
            exit(Sif::ErrorCode::JSON_AST_FILE_NOT_PROVIDED);
        }
        if (result.count("visitor_arg")) {
            visitor_arg = result["visitor_arg"].as<std::string>();
        } else {
            visitor_arg = "";
        }
        if (result.count("output")) {
            output_file_name = result["output"].as<std::string>();
        }
        if (result.count("silent")) {
            silent_mode = result["silent"].as<bool>();
        }

        //if (result.count("abi")) {
        //    abi_file_name = result["abi"].as<std::string>();
        //} else {
        //    std::cout << "ABI file not provided\n";
        //    exit(Sif::ErrorCode::ABI_FILE_NOT_PROVIDED);
        //}
    } catch (const cxxopts::OptionException& e) {
        std::cout << "error parsing options: " << e.what() << "\n";
        exit(Sif::ErrorCode::COMMAND_LINE_ARGUMENTS_ERROR);
    }

    std::ifstream ast_text_file_stream(ast_file_name), ast_json_file_stream(ast_json_file_name);
    std::stringstream ast_text_stream, abi_stream;
    std::string new_line, sol_name, ast_json_content;

    std::ifstream ast_text_file_stream2(ast_file_name2), ast_json_file_stream2(ast_json_file_name2);
    std::stringstream ast_text_stream2, abi_stream2;
    std::string new_line2, sol_name2, ast_json_content2;

    while (getline(ast_text_file_stream, new_line)) {
        ast_text_stream << new_line << "\n";
    }

    while (getline(ast_text_file_stream2, new_line2)) {
        ast_text_stream2 << new_line2 << "\n";
    }

    while (getline(ast_json_file_stream, new_line)) {
        if (new_line.find(".sol =======") != std::string::npos) {
            sol_name = Sif::Utils::substr_by_edge(new_line, "======= ", " =======");
            break;
        }
    }

    while (getline(ast_json_file_stream2, new_line2)) {
        if (new_line2.find(".sol =======") != std::string::npos) {
            sol_name2 = Sif::Utils::substr_by_edge(new_line2, "======= ", " =======");
            break;
        }
    }

    while (getline(ast_json_file_stream, new_line)) {
        if (new_line.find(".sol =======") == std::string::npos) {
            ast_json_content = ast_json_content + new_line + "\n";
        } else {
            nlohmann::json ast_json = nlohmann::json::parse(ast_json_content);
            //Sif::ASTAnalyser ast_analyser(ast_json, sol_name);
            //std::stringstream new_source = ast_analyser.analyse();
            //std::cout << sol_name << " " << ast_json.at("absolutePath") << std::endl;
            sol_name = Sif::Utils::substr_by_edge(new_line, "", "");
            ast_json_content = "";
        }
    }

    while (getline(ast_json_file_stream2, new_line2)) {
        if (new_line2.find(".sol =======") == std::string::npos) {
            ast_json_content2 = ast_json_content2 + new_line2 + "\n";
        } else {
            nlohmann::json ast_json2 = nlohmann::json::parse(ast_json_content2);
            //Sif::ASTAnalyser ast_analyser(ast_json, sol_name);
            //std::stringstream new_source = ast_analyser.analyse();
            //std::cout << sol_name << " " << ast_json.at("absolutePath") << std::endl;
            sol_name2 = Sif::Utils::substr_by_edge(new_line2, "", "");
            ast_json_content2 = "";
        }
    }

    Sif::Indentation empty(0);
    Sif::RootNodePtr root_node, root_node2;

    if (ast_json_content != "") {
        nlohmann::json ast_json = nlohmann::json::parse(ast_json_content);
        Sif::ASTAnalyser ast_analyser(ast_text_stream, ast_json, true, sol_name, visitor_arg);
        root_node = ast_analyser.analyse();
        
    }

    if (ast_json_content2 != "") {
        nlohmann::json ast_json2 = nlohmann::json::parse(ast_json_content2);
        Sif::ASTAnalyser ast_analyser(ast_text_stream2, ast_json2, true, sol_name2, visitor_arg);
        root_node2 = ast_analyser.analyse();
    }
    
    ast_json_file_stream.close();
    ast_json_file_stream2.close();
    global_solname1 = sol_name;
    global_solname2 = sol_name2;

    Sif::Indentation indentation;
    {
        using namespace Sif;
        if (root_node->get_import() != root_node2->get_import()) {
            report_issue("Import Directive", root_node->get_import(), root_node2->get_import());
        }

        if (root_node->get_pragma() != root_node2->get_pragma()) {
            report_issue("Pragma Directive", root_node->get_pragma(), root_node2->get_pragma());
        }


        for (int i = 0; i < root_node->num_fields(); ++i) {
            ContractDefinitionNodePtr cdn = std::dynamic_pointer_cast<ContractDefinitionNode>(root_node->get_field(i));
            std::string contract_name = cdn->get_name();
            bool found = false;
            ContractDefinitionNodePtr cdn2;
            for (int j = 0; j < root_node2->num_fields(); ++j) {
                cdn2 = std::dynamic_pointer_cast<ContractDefinitionNode>(root_node2->get_field(j));
                if (cdn2->get_name() == contract_name) {
                    found = true;
                    break;
                }
            }
            if (found) {
                if (cdn->contract_is_library() != cdn2->contract_is_library()) {
                    report_issue("Library specifier for " + contract_name, cdn->contract_is_library(), cdn2->contract_is_library());
                }
                for (int funcI = 0; funcI < cdn->num_members(); ++funcI) {
                    ASTNodePtr cdn1Member = cdn->get_member(funcI);
                    if (cdn1Member->get_node_type() == NodeTypeFunctionDefinition) {
                        FunctionDefinitionNodePtr fdn1 = std::dynamic_pointer_cast<FunctionDefinitionNode>(cdn->get_member(funcI));
                        std::string funcName1 = fdn1->get_name();

                        bool funcFound = false;
                        FunctionDefinitionNodePtr fdn2;
                        for (int funcJ = 0; funcJ < cdn2->num_members(); ++ funcJ) {
                            auto cdn2Member = cdn2->get_member(funcJ);
                            if (cdn2Member->get_node_type() == NodeTypeFunctionDefinition) {
                                fdn2 = std::dynamic_pointer_cast<FunctionDefinitionNode>(cdn2->get_member(funcJ));
                                std::string funcName2 = fdn2->get_name();
                                if (funcName1 == funcName2) {
                                    funcFound = true;
                                    if (fdn1->function_is_constructor() != fdn2->function_is_constructor()) {
                                        report_issue("Constructor specifier for " + funcName1, fdn1->function_is_constructor(), fdn1->function_is_constructor());
                                    }
                                    break;
                                }
                            }
                            
                        }
                        if (funcFound) {
                            if (fdn1->get_params()!=nullptr && fdn2->get_params()==nullptr) {
                                report_issue("Parameter List of Function [" + funcName1 + "]", fdn1->get_params()->source_code(empty), "------------");
                            }
                            else if (fdn2->get_params()!=nullptr && fdn1->get_params()==nullptr) {
                                report_issue("Parameter List of Function [" + funcName1 + "]", "------------", fdn2->get_params()->source_code(empty));
                            }
                            if (fdn1->get_returns()!=nullptr && fdn2->get_returns()==nullptr) {
                                report_issue("Parameter List of Function [" + funcName1 + "]", fdn1->get_returns()->source_code(empty), "------------");
                            }
                            else if (fdn2->get_returns()!=nullptr && fdn1->get_returns()==nullptr) {
                                report_issue("Return List of Function [" + funcName1 + "]", "------------", fdn2->get_returns()->source_code(empty));
                            }
                            report_issue("Parameter List of Function [" + funcName1 + "]", fdn1->get_params()->source_code(empty), fdn2->get_params()->source_code(empty));
                            report_issue("Return List of Function [" + funcName1 + "]", fdn1->get_returns()->source_code(empty), fdn2->get_returns()->source_code(empty));
                            auto body1 = fdn1->get_function_body();
                            auto body2 = fdn2->get_function_body();
                            std::vector<std::string> stmts1;
                            std::vector<std::string> stmts2;

                            for (int stmtI = 0; stmtI < body1->num_statements(); ++stmtI) {
                                std::string stmt1 = body1->get_statement(stmtI)->source_code(empty);
                                stmts1.push_back(stmt1);
                            }
                            for (int stmtJ = 0; stmtJ < body2->num_statements(); ++stmtJ) {
                                std::string stmt2 = body2->get_statement(stmtJ)->source_code(empty);
                                stmts2.push_back(stmt2);
                            }

                            auto lastIt2Pos = stmts2.begin();
                            auto lastFoundPos = stmts2.begin();
                            for (auto it1 = stmts1.begin(); it1 != stmts1.end(); ++it1) {
                                bool stmtFound = false;
                                auto it2 = lastIt2Pos;
                                for (; it2  != stmts2.end(); ++it2) {
                                    if (*it1 == *it2) {
                                        stmtFound = true;
                                        lastIt2Pos = it2 + 1;
                                        break;
                                    }
                                }
                                if (stmtFound) {
                                    if (lastFoundPos + 1 != it2 && lastFoundPos != it2) {
                                        for (auto itRemaining = lastFoundPos + 1; itRemaining != it2; ++itRemaining) {
                                            report_issue("Statement", "------------", *itRemaining);
                                        }
                                    }
                                    lastFoundPos = it2;
                                } else {
                                    report_issue("Statement", *it1, "--------");
                                }
                            }
                            
                            for (auto posStmts2 = lastIt2Pos; posStmts2 != stmts2.end(); ++posStmts2) {
                                report_issue("Statement", "------------", *posStmts2);
                            }
                            /* 
                            int stmtJLastPos = 0;
                            for (int stmtI = 0; stmtI < body1->num_statements(); ++stmtI) {
                                std::string stmt1 = body1->get_statement(stmtI)->source_code(empty);
                                bool stmtFound = false;
                                for (int stmtJ = stmtJLastPos; stmtJ < body2->num_statements(); ++ stmtJ) {
                                    std::string stmt2 = body2->get_statement(stmtJ)->source_code(empty);
                                    if (stmt1 == stmt2) {
                                        ++stmtJLastPos;
                                        stmtFound = true;
                                        break;
                                    }
                                }
                                if (!stmtFound) {
                                    report_issue("Statement", stmt1, "--------");
                                }
                            }
                            for (int stmtJ = stmtJLastPos; stmtJ < body2->num_statements(); ++ stmtJ) {
                                std::string stmt2 = body2->get_statement(stmtJ)->source_code(empty);
                                report_issue("Statement", "--------", stmt2);
                            }
                            */
                        } else {
                            std::cout << "Function [" + funcName1 + "] in Contract [" + contract_name + "] not found in " + sol_name2 + "\n";
                        }
                    } else if (cdn1Member->get_node_type() == NodeTypeVariableDeclaration) {
                        auto fdn1 = std::dynamic_pointer_cast<VariableDeclarationNode>(cdn->get_member(funcI));
                        std::string funcName1 = fdn1->get_variable_name();

                        bool funcFound = false;
                        VariableDeclarationNodePtr fdn2;
                        for (int funcJ = 0; funcJ < cdn2->num_members(); ++ funcJ) {
                            auto cdn2Member = cdn2->get_member(funcJ);
                            if (cdn2Member->get_node_type() == NodeTypeVariableDeclaration) {
                                fdn2 = std::dynamic_pointer_cast<VariableDeclarationNode>(cdn2->get_member(funcJ));
                                std::string funcName2 = fdn2->get_variable_name();
                                if (funcName1 == funcName2) {
                                    funcFound = true;
                                    report_issue("Variable Type of [" + funcName1 + "]", fdn1->get_type()->source_code(empty), fdn2->get_type()->source_code(empty));
                                    if (fdn1->get_initial_value() || fdn2->get_initial_value()) {
                                        if (fdn1->get_initial_value() && fdn2->get_initial_value()) {
                                            report_issue("Variable Init of [" + funcName1 + "]", fdn1->get_initial_value()->source_code(empty), fdn2->get_initial_value()->source_code(empty));
                                        } else if (fdn1->get_initial_value()) {
                                            report_issue("Variable Init of [" + funcName1 + "]", fdn1->get_initial_value()->source_code(empty), "------------");
                                        } else if (fdn2->get_initial_value()) {
                                            report_issue("Variable Init of [" + funcName1 + "]", "------------", fdn2->get_initial_value()->source_code(empty));
                                        }
                                    }
                                    break;
                                }
                            }
                            
                        }
                        if (!funcFound) {
                            std::cout << "Variable [" + funcName1 + "] in Contract [" + contract_name + "] not found in " + sol_name2 + "\n";
                        } 
                    } else if (cdn1Member->get_node_type() == NodeTypeEventDefinition) {
                        auto fdn1 = std::dynamic_pointer_cast<EventDefinitionNode>(cdn->get_member(funcI));
                        std::string funcName1 = fdn1->get_name();

                        bool funcFound = false;
                        EventDefinitionNodePtr fdn2;
                        for (int funcJ = 0; funcJ < cdn2->num_members(); ++ funcJ) {
                            auto cdn2Member = cdn2->get_member(funcJ);
                            if (cdn2Member->get_node_type() == NodeTypeEventDefinition) {
                                fdn2 = std::dynamic_pointer_cast<EventDefinitionNode>(cdn2->get_member(funcJ));
                                std::string funcName2 = fdn2->get_name();
                                if (funcName1 == funcName2) {
                                    funcFound = true;
                                    if (fdn1->get_argument_list() || fdn2->get_argument_list()) {
                                        if (fdn1->get_argument_list() && fdn2->get_argument_list()) {
                                            report_issue("Parameter List of Event [" + funcName1 + "]", fdn1->get_argument_list()->source_code(empty), fdn2->get_argument_list()->source_code(empty));
                                        } else if (fdn1->get_argument_list()) {
                                            report_issue("Parameter List of Event [" + funcName1 + "]", fdn1->get_argument_list()->source_code(empty), "------------");
                                        } else if (fdn2->get_argument_list()) {
                                            report_issue("Parameter List of Event [" + funcName1 + "]", "------------", fdn2->get_argument_list()->source_code(empty));
                                        }
                                    }
                                    break;
                                }
                            }
                            
                        }
                        if (!funcFound) {
                            std::cout << "Event [" + funcName1 + "] in Contract [" + contract_name + "] not found in " + sol_name2 + "\n";
                        } 
                    } else if (cdn1Member->get_node_type() == NodeTypeUsingForDirective) {
                        auto fdn1 = std::dynamic_pointer_cast<UsingForDirectiveNode>(cdn->get_member(funcI));
                        std::string funcName1 = fdn1->get_for();

                        bool funcFound = false;
                        UsingForDirectiveNodePtr fdn2;
                        for (int funcJ = 0; funcJ < cdn2->num_members(); ++ funcJ) {
                            auto cdn2Member = cdn2->get_member(funcJ);
                            if (cdn2Member->get_node_type() == NodeTypeUsingForDirective) {
                                fdn2 = std::dynamic_pointer_cast<UsingForDirectiveNode>(cdn2->get_member(funcJ));
                                std::string funcName2 = fdn2->get_for();
                                if (funcName1 == funcName2) {
                                    funcFound = true;
                                    if (fdn1->get_using() != fdn2->get_using())
                                        report_issue("Using For Directive", fdn1->source_code(empty), fdn2->source_code(empty));
                                    break;
                                }
                            }
                            
                        }
                        if (!funcFound) {
                            std::cout << "Using For Directive [" + funcName1 + "] in Contract [" + contract_name + "] not found in " + sol_name2 + "\n";
                        } 
                    } else if (cdn1Member->get_node_type() == NodeTypeModifierDefinition) {
                        auto fdn1 = std::dynamic_pointer_cast<ModifierDefinitionNode>(cdn->get_member(funcI));
                        std::string funcName1 = fdn1->get_name();

                        bool funcFound = false;
                        ModifierDefinitionNodePtr fdn2;
                        for (int funcJ = 0; funcJ < cdn2->num_members(); ++ funcJ) {
                            auto cdn2Member = cdn2->get_member(funcJ);
                            if (cdn2Member->get_node_type() == NodeTypeModifierDefinition) {
                                fdn2 = std::dynamic_pointer_cast<ModifierDefinitionNode>(cdn2->get_member(funcJ));
                                std::string funcName2 = fdn2->get_name();
                                if (funcName1 == funcName2) {
                                    funcFound = true;
                                    if (fdn1->get_params()!=nullptr && fdn2->get_params()==nullptr) {
                                        report_issue("Parameter List of Modifier [" + funcName1 + "]", fdn1->get_params()->source_code(empty), "------------");
                                    }
                                    else if (fdn2->get_params()!=nullptr && fdn1->get_params()==nullptr) {
                                        report_issue("Parameter List of Modifier [" + funcName1 + "]", "------------", fdn2->get_params()->source_code(empty));
                                    }
                                    report_issue("Parameter List of Modifier [" + funcName1 + "]", fdn1->get_params()->source_code(empty), fdn2->get_params()->source_code(empty));
                                    report_issue("Statement of Modifier [" + funcName1 + "]", fdn1->get_body()->source_code(empty), fdn2->get_body()->source_code(empty));
                                }
                            }
                            
                        }
                        if (!funcFound) {
                            std::cout << "Modifier [" + funcName1 + "] in Contract [" + contract_name + "] not found in " + sol_name2 + "\n";
                        } 
                    } else if (cdn1Member->get_node_type() == NodeTypeStructDefinition) {
                        StructDefinitionNodePtr fdn1 = std::dynamic_pointer_cast<StructDefinitionNode>(cdn->get_member(funcI));
                        std::string funcName1 = fdn1->get_name();

                        bool funcFound = false;
                        StructDefinitionNodePtr fdn2;
                        for (int funcJ = 0; funcJ < cdn2->num_members(); ++ funcJ) {
                            auto cdn2Member = cdn2->get_member(funcJ);
                            if (cdn2Member->get_node_type() == NodeTypeStructDefinition) {
                                fdn2 = std::dynamic_pointer_cast<StructDefinitionNode>(cdn2->get_member(funcJ));
                                std::string funcName2 = fdn2->get_name();
                                if (funcName1 == funcName2) {
                                    funcFound = true;
                                    break;
                                }
                            }
                            
                        }
                        if (funcFound) {
                            std::vector<std::string> stmts1;
                            std::vector<std::string> stmts2;
                            for (int stmtI = 0; stmtI < fdn1->num_fields(); ++stmtI) {
                                std::string stmt1 = fdn1->get_field(stmtI)->source_code(empty);
                                stmts1.push_back(stmt1);
                            }
                            for (int stmtJ = 0; stmtJ < fdn2->num_fields(); ++stmtJ) {
                                std::string stmt2 = fdn2->get_field(stmtJ)->source_code(empty);
                                stmts2.push_back(stmt2);
                            }

                            auto lastIt2Pos = stmts2.begin();
                            auto lastFoundPos = stmts2.begin();
                            for (auto it1 = stmts1.begin(); it1 != stmts1.end(); ++it1) {
                                bool stmtFound = false;
                                auto it2 = lastIt2Pos;
                                for (; it2  != stmts2.end(); ++it2) {
                                    if (*it1 == *it2) {
                                        stmtFound = true;
                                        lastIt2Pos = it2 + 1;
                                        break;
                                    }
                                }
                                if (stmtFound) {
                                    if (lastFoundPos + 1 != it2 && lastFoundPos != it2) {
                                        for (auto itRemaining = lastFoundPos + 1; itRemaining != it2; ++itRemaining) {
                                            report_issue("Struct Member of [" + funcName1 + "]", "------------", *itRemaining);
                                        }
                                    }
                                    lastFoundPos = it2;
                                } else {
                                    report_issue("Struct Member of [" + funcName1 + "]", *it1, "--------");
                                }
                            }
                            
                            for (auto posStmts2 = lastIt2Pos; posStmts2 != stmts2.end(); ++posStmts2) {
                                report_issue("Struct Member of [" + funcName1 + "]", "------------", *posStmts2);
                            }
                            /* 
                            int stmtJLastPos = 0;
                            for (int stmtI = 0; stmtI < body1->num_statements(); ++stmtI) {
                                std::string stmt1 = body1->get_statement(stmtI)->source_code(empty);
                                bool stmtFound = false;
                                for (int stmtJ = stmtJLastPos; stmtJ < body2->num_statements(); ++ stmtJ) {
                                    std::string stmt2 = body2->get_statement(stmtJ)->source_code(empty);
                                    if (stmt1 == stmt2) {
                                        ++stmtJLastPos;
                                        stmtFound = true;
                                        break;
                                    }
                                }
                                if (!stmtFound) {
                                    report_issue("Statement", stmt1, "--------");
                                }
                            }
                            for (int stmtJ = stmtJLastPos; stmtJ < body2->num_statements(); ++ stmtJ) {
                                std::string stmt2 = body2->get_statement(stmtJ)->source_code(empty);
                                report_issue("Statement", "--------", stmt2);
                            }
                            */
                        } else {
                            std::cout << "Struct [" + funcName1 + "] in Contract [" + contract_name + "] not found in " + sol_name2 + "\n";
                        }
                    }
                    
                }

                for (int funcJ = 0; funcJ < cdn2->num_members(); ++funcJ) {
                    auto cdn2Member = cdn2->get_member(funcJ);
                    if (cdn2Member->get_node_type() == NodeTypeFunctionDefinition) {
                        FunctionDefinitionNodePtr fdn2 = std::dynamic_pointer_cast<FunctionDefinitionNode>(cdn2Member);
                        std::string funcName2 = fdn2->get_name();
                        bool funcFound = false;
                        for (int funcI = 0; funcI < cdn->num_members(); ++ funcI) {
                            if (cdn->get_member(funcI)->get_node_type() == NodeTypeFunctionDefinition) {
                                FunctionDefinitionNodePtr fdn1 = std::dynamic_pointer_cast<FunctionDefinitionNode>(cdn->get_member(funcI));
                                std::string funcName1 = fdn1->get_name();
                                if (funcName1 == funcName2) {
                                    funcFound = true;
                                    break;
                                }
                            }
                            
                        }
                        if (!funcFound) {
                            std::cout << "Function [" + funcName2 + "] in Contract [" + contract_name + "] not found in " + sol_name + "\n";
                        }
                    } else if (cdn2Member->get_node_type() == NodeTypeVariableDeclaration) {
                        auto fdn2 = std::dynamic_pointer_cast<VariableDeclarationNode>(cdn2Member);
                        std::string funcName2 = fdn2->get_variable_name();
                        bool funcFound = false;
                        for (int funcI = 0; funcI < cdn->num_members(); ++ funcI) {
                            if (cdn->get_member(funcI)->get_node_type() == NodeTypeVariableDeclaration) {
                                auto fdn1 = std::dynamic_pointer_cast<VariableDeclarationNode>(cdn->get_member(funcI));
                                std::string funcName1 = fdn1->get_variable_name();
                                if (funcName1 == funcName2) {
                                    funcFound = true;
                                    break;
                                }
                            }
                        }
                        if (!funcFound) {
                            std::cout << "Variable [" + funcName2 + "] in Contract [" + contract_name + "] not found in " + sol_name + "\n";
                        }
                    } else if (cdn2Member->get_node_type() == NodeTypeEventDefinition) {
                        auto fdn2 = std::dynamic_pointer_cast<EventDefinitionNode>(cdn2Member);
                        std::string funcName2 = fdn2->get_name();
                        bool funcFound = false;
                        for (int funcI = 0; funcI < cdn->num_members(); ++ funcI) {
                            if (cdn->get_member(funcI)->get_node_type() == NodeTypeEventDefinition) {
                                auto fdn1 = std::dynamic_pointer_cast<EventDefinitionNode>(cdn->get_member(funcI));
                                std::string funcName1 = fdn1->get_name();
                                if (funcName1 == funcName2) {
                                    funcFound = true;
                                    break;
                                }
                            }
                        }
                        if (!funcFound) {
                            std::cout << "Event [" + funcName2 + "] in Contract [" + contract_name + "] not found in " + sol_name + "\n";
                        }
                    } else if (cdn2Member->get_node_type() == NodeTypeUsingForDirective) {
                        auto fdn2 = std::dynamic_pointer_cast<UsingForDirectiveNode>(cdn2Member);
                        std::string funcName2 = fdn2->get_for();
                        bool funcFound = false;
                        for (int funcI = 0; funcI < cdn->num_members(); ++ funcI) {
                            if (cdn->get_member(funcI)->get_node_type() == NodeTypeUsingForDirective) {
                                auto fdn1 = std::dynamic_pointer_cast<UsingForDirectiveNode>(cdn->get_member(funcI));
                                std::string funcName1 = fdn1->get_for();
                                if (funcName1 == funcName2) {
                                    funcFound = true;
                                    break;
                                }
                            }
                        }
                        if (!funcFound) {
                            std::cout << "Using For Directive [" + funcName2 + "] in Contract [" + contract_name + "] not found in " + sol_name + "\n";
                        }
                    } else if (cdn2Member->get_node_type() == NodeTypeModifierDefinition) {
                        auto fdn2 = std::dynamic_pointer_cast<ModifierDefinitionNode>(cdn2Member);
                        std::string funcName2 = fdn2->get_name();
                        bool funcFound = false;
                        for (int funcI = 0; funcI < cdn->num_members(); ++ funcI) {
                            if (cdn->get_member(funcI)->get_node_type() == NodeTypeModifierDefinition) {
                                auto fdn1 = std::dynamic_pointer_cast<ModifierDefinitionNode>(cdn->get_member(funcI));
                                std::string funcName1 = fdn1->get_name();
                                if (funcName1 == funcName2) {
                                    funcFound = true;
                                    break;
                                }
                            }
                        }
                        if (!funcFound) {
                            std::cout << "Modifier [" + funcName2 + "] in Contract [" + contract_name + "] not found in " + sol_name + "\n";
                        }
                    } else if (cdn2Member->get_node_type() == NodeTypeStructDefinition) {
                        auto fdn2 = std::dynamic_pointer_cast<StructDefinitionNode>(cdn2Member);
                        std::string funcName2 = fdn2->get_name();
                        bool funcFound = false;
                        for (int funcI = 0; funcI < cdn->num_members(); ++ funcI) {
                            if (cdn->get_member(funcI)->get_node_type() == NodeTypeStructDefinition) {
                                auto fdn1 = std::dynamic_pointer_cast<StructDefinitionNode>(cdn->get_member(funcI));
                                std::string funcName1 = fdn1->get_name();
                                if (funcName1 == funcName2) {
                                    funcFound = true;
                                    break;
                                }
                            } 
                        }
                        if (!funcFound) {
                            std::cout << "Struct [" + funcName2 + "] in Contract [" + contract_name + "] not found in " + sol_name + "\n";
                        }
                    }
                    
                }
            } else {
                std::cout << "Contract [" + contract_name + "] not found in " << sol_name2 << "\n";
            }
        }

        for (int j = 0; j < root_node2->num_fields(); ++j) {
            ContractDefinitionNodePtr cdn2 = std::dynamic_pointer_cast<ContractDefinitionNode>(root_node2->get_field(j));
            std::string contract_name = cdn2->get_name();
            bool found = false;
            for (int i = 0; i < root_node->num_fields(); ++i) {
                ContractDefinitionNodePtr cdn = std::dynamic_pointer_cast<ContractDefinitionNode>(root_node->get_field(i));
                if (cdn->get_name() == contract_name) {
                    found = true;
                    break;
                }
            }
            if (!found) {
                std::cout << "Contract [" + contract_name + "] not found in " << sol_name << "\n";
            }
        }
    }
    

    exit(Sif::ErrorCode::SUCCESS);

    


    /*
    Code debugging the json ast
    std::cout << jsonast.dump();
    std::cout << jsonast.at("nodes").at(2).at("nodes").at(0).at("visibility") << std::endl;
    std::cout << jsonast.at("nodes").at(2).at("nodes").at(0).at("visibility") << std::endl;
    std::cout << jsonast.at("nodes").at(8).at("nodes").at(5).at("name") << std::endl;
    std::cout << jsonast.at("nodes").at(8).at("nodes").at(5).at("visibility") << std::endl;
    std::cout << jsonast.at("nodes").at(8).at("nodes").at(5).at("stateMutability") << std::endl;
    std::cout << jsonast.at("nodes").at(8).at("nodes").at(4).at("isConstructor") << std::endl;
    std::cout << jsonast.at("nodes").at(7).at("nodes").at(5).at("visibility") << std::endl;
    */

    
    
    

    return 0;
}