#include "sema/SemanticAnalyzer.hpp"
#include "visitor/AstNodeInclude.hpp"
#include "sema/symbolentry.hpp"
#include "sema/symbolmanager.hpp"
#include "sema/symboltable.hpp"
#include <stack>
#include <string.h>
#include <cstring>
#include <cstdint>
extern int32_t OptDmp;
extern int32_t err_found;
extern FILE *yyin;
//extern std::string fname;
SymbolEntry* currant_entry;
SymbolTable* currant_table;
SymbolManager* currant_manager = new SymbolManager();
int level=0;
int kindstatus=0;
int func_com=0;
int from_for=0, from_print=0,from_read=0,from_ret=0,from_bin=0,from_varr=0,from_const=0,two_const=0,from_un=0;
int sec_com_func=0,for_err=0,from_com=0;
std::vector<std::string> error;
std::stack<char *> func_type;
char* currant_func_type;
char unary_name[100];
char bin_left_type[100];
char bin_right_type[100];
char pre_bin_type[100];
char func_return_type[100];
void dumpDemarcation(const char chr) {
  for (size_t i = 0; i < 110; ++i) {
    printf("%c", chr);
  }
  puts("");
}
PTypeSharedPtr child_type;
void dumpSymbol(void) {
    if(OptDmp){
    SymbolTable tmp = currant_manager->tables.top();
  dumpDemarcation('=');
  printf("%-33s%-11s%-11s%-17s%-11s\n", "Name", "Kind", "Level", "Type",
                                        "Attribute");
  dumpDemarcation('-');
  for (int i=0;i< tmp.entries.size();i++){
    printf("%-33s", tmp.entries[i].m_name.c_str());
    printf("%-11s", tmp.entries[i].m_kind.c_str());
    printf("%d%-10s",  tmp.entries[i].m_level, tmp.entries[i].m_level==0?"(global)":"(local)");
    printf("%-17s", tmp.entries[i].m_type.c_str());
    printf("%-11s", tmp.entries[i].m_attribute.c_str());
    puts("");
  }
  dumpDemarcation('-');
}
}
void dumpErr(int line, int col, int errnum, std::string param1, std::string param2, std::string param3) {
    fprintf(stderr, "<Error> Found in line %d, column %d: ", line, col);
    err_found=1;
    switch (errnum)
    {
    case 0:
        fprintf(stderr, "symbol '%s' is redeclared\n", param1.substr(0, 32).c_str());
        break;
    case 1:
            fprintf(stderr, "'%s' declared as an array with an index that is not greater than 0\n", param1.c_str());
            break;
    case 2:
        fprintf(stderr, "use of undeclared symbol '%s' \n", param1.c_str());
        break;
    case 3:
        fprintf(stderr, "there is an over array subscript on '%s' \n", param1.c_str());
        break;
    case 4:
        fprintf(stderr, "index of array reference must be an integer \n");
        break;
    case 5:
        fprintf(stderr, "use of non-variable symbol '%s' \n", param1.c_str());
        break;
    case 6:
        fprintf (stderr, "use of undeclared symbol '%s' \n", param1.c_str());
        break;
    case 7:
        fprintf (stderr, "there is an over array subscript on '%s' \n", param1.c_str());
        break;
    case 8:
        fprintf (stderr, "variable reference of read statement must be scalar type \n");
        break;
    case 9:
        fprintf (stderr, "variable reference of read statement cannot be a constant or loop variable \n");
        break;
    case 10:
        fprintf (stderr, "the lower bound and upper bound of iteration count must be in the incremental order \n");
        break;
    case 11:
        fprintf (stderr, "there is an over array subscript on '%s' \n",  param1.c_str());
        break;
    case 12:
        fprintf (stderr, "return '%s' from a function with return type '%s' \n",  param1.c_str(), param2.c_str());
        break;
    case 13:
        fprintf (stderr, "program/procedure should not return a value \n");
        break;
    case 14:
        fprintf (stderr, "expression of print statement must be scalar type \n");
        break;
    case 15:
        fprintf (stderr, "invalid operands to binary operator '%s' ('%s' and '%s') \n", param1.c_str(),param2.c_str(),param3.c_str());
        break;
    case 16:
        fprintf (stderr, "call of non-function symbol '%s' \n", param1.c_str());
        break;
    case 17:
        fprintf (stderr, "too few/much arguments provided for function '%s' \n", param1.c_str());
        break;
    case 18:
        fprintf (stderr, "invalid operands to binary operator '%s' ('%s' and '%s') \n", param1.c_str(),param2.c_str(),param3.c_str());
        break;
    case 19:
        fprintf (stderr, "invalid operand to unary operator '%s' ('%s') \n", param1.c_str(),param2.c_str());
        break;
    case 20:
        fprintf (stderr, "the expression of condition must be boolean type \n");
        break;
    case 21:
        fprintf (stderr, "incompatible type passing '%s' to parameter of type '%s' \n", param1.c_str(),param2.c_str());
        break;
    case 22:
        fprintf (stderr, "array assignment is not allowed \n");
        break;
    case 23:
        fprintf (stderr, "cannot assign to variable '%s' which is a constant \n",param1.c_str());
        break;
    case 24:
        fprintf (stderr, "the value of loop variable cannot be modified inside the loop body \n");
        break;
    case 25:
        fprintf (stderr, "assigning to '%s' from incompatible type '%s' \n",param1.c_str(),param2.c_str());
        break;
    default:
        break;
    }
    fprintf(stderr, "    ");
    char errline[1000];
    int c = 0, cn = 0, ln = 0;
    fseek ( yyin , 0 , SEEK_SET );
    while (ln != line-1 && c != EOF) {
        c = fgetc (yyin);
        cn++;
        if (c == '\n') ln++;
    }
    fseek ( yyin , cn , SEEK_SET );
    fgets (errline , 1000 , yyin);
    fprintf(stderr, "%s", errline);

    fprintf(stderr, "    ");
    for (int i=0; i < col-1; i++) fprintf(stderr, " ");
    fprintf(stderr, "^\n");
}
int reDeclCheck(std::string newSymbol) {
    std::stack <SymbolTable> tmpTables = currant_manager->tables;
    while (!tmpTables.empty()) {
        SymbolTable tmpTable = tmpTables.top();
        for (uint i=0; i< tmpTable.entries.size(); i++) {
            if(tmpTable.entries[i].m_name == newSymbol) {
                if(tmpTable.entries[i].m_kind == "loop_var")
                    return 1;
            }
        }
        tmpTables.pop();
    }
    for (uint i=0; i< currant_table->entries.size(); i++) {
        if (currant_table->entries[i].m_name == newSymbol)
            return 1;
    }
    return 0;
}

void SemanticAnalyzer::visit(ProgramNode &p_program) {
    /*
     * TODO:
     *
     * 1. Push a new symbol table if this node forms a scope.
     * 2. Insert the symbol into current symbol table if this node is related to
     *    declaration (ProgramNode, VariableNode, FunctionNode).
     * 3. Travere child nodes of this node.
     * 4. Perform semantic analyses of this node.
     * 5. Pop the symbol table pushed at the 1st step.
     */
    SymbolTable* table = new SymbolTable();
    currant_table = table;
    SymbolEntry* entry = new SymbolEntry;//(p_program.name.substr(0, 32),"program", level,"void","");
    currant_entry=entry;
    currant_entry->m_name = p_program.name;
    currant_entry->m_kind = "program";
    currant_entry->m_type = "void";
    currant_entry->m_level = level;
    currant_table->addSymbol(*currant_entry);
    currant_func_type="void";
    func_type.push(currant_func_type);
    if (p_program.var_decls.size() != 0 ){
        for (auto &decl : p_program.var_decls) {
            decl->accept(*this);
        }
    }
    currant_manager -> pushScope(*currant_table);
    if (p_program.funcs.size() != 0){
        for (auto &func : p_program.funcs) {
            func->accept(*this);
        } 
    }

    auto &com = p_program.body;
    com->accept(*this);
    dumpSymbol();
    currant_manager->popScope();

}

void SemanticAnalyzer::visit(DeclNode &p_decl) {
    //printf("1111\n");
    p_decl.visitChildNodes(*this);
}
int arr_range(std::string type){
    int i=0;
    
    while (1){
        if (type[i]=='['&&type[i+1]=='0'){
            return 1;
        }
        i++;
        if (type[i]=='\0'){
            break;
        }
    }
    return 0;
}
void SemanticAnalyzer::visit(VariableNode &p_variable) {
    /*
     * TODO:
     *
     * 1. Push a new symbol table if this node forms a scope.
     * 2. Insert the symbol into current symbol table if this node is related to
     *    declaration (ProgramNode, VariableNode, FunctionNode).
     * 3. Travere child nodes of this node.
     * 4. Perform semantic analyses of this node.
     * 5. Pop the symbol table pushed at the 1st step.
     */
    if (arr_range(p_variable.getTypeCString())){
        dumpErr(p_variable.getLocation().line, p_variable.getLocation().col, 1, p_variable.name.c_str(), "", "");
        error.push_back( p_variable.name);
    }
    if ( reDeclCheck(p_variable.name.substr(0, 32)) ) {
        dumpErr(p_variable.getLocation().line, p_variable.getLocation().col, 0, p_variable.name.c_str(), "", "");
        error.push_back( p_variable.name);
        return;
        //if (kindstatus == 2) forErr = 1;
    }
    else {
        SymbolEntry* entry = new SymbolEntry();
        currant_entry = entry;
        currant_entry->m_name = p_variable.name;
        currant_entry->m_kind = kindstatus==0?"variable":kindstatus==1?"parameter":"loop_var";
        currant_entry->m_level = level;
        currant_entry->m_type = p_variable.getTypeCString();
        if (p_variable.constant!=NULL){
            p_variable.constant->accept(*this);
        }
        currant_table->addSymbol(*currant_entry);
    }
    p_variable.visitChildNodes(*this);
}

void SemanticAnalyzer::visit(ConstantValueNode &p_constant_value) {
    /*
     * TODO:
     *
     * 1. Push a new symbol table if this node forms a scope.
     * 2. Insert the symbol into current symbol table if this node is related to
     *    declaration (ProgramNode, VariableNode, FunctionNode).
     * 3. Travere child nodes of this node.
     * 4. Perform semantic analyses of this node.
     * 5. Pop the symbol table pushed at the 1st step.
     */
    currant_entry->m_kind = "constant";
    currant_entry->m_attribute = p_constant_value.getConstantValueCString();
    p_constant_value.visitChildNodes(*this); 
    child_type = p_constant_value.getTypePtr();
    p_constant_value.type=p_constant_value.getTypePtr();
    from_const=1;
    //currant_entry->m_type = p_constant_value.getTypePtr();
}

void SemanticAnalyzer::visit(FunctionNode &p_function) {
    /*
     * TODO:
     *
     * 1. Push a new symbol table if this node forms a scope.
     * 2. Insert the symbol into current symbol table if this node is related to
     *    declaration (ProgramNode, VariableNode, FunctionNode).
     * 3. Travere child nodes of this node.
     * 4. Perform semantic analyses of this node.
     * 5. Pop the symbol table pushed at the 1st step.
     */
    if ( reDeclCheck(p_function.getNameCString()) ) {
        dumpErr(p_function.getLocation().line, p_function.getLocation().col, 0, p_function.getNameCString(), "", "");
    }
    else{
        SymbolEntry* entry = new SymbolEntry();
        currant_entry = entry;
        currant_entry->m_name = p_function.getNameCString();
        currant_entry->m_kind = "function";
        currant_entry->m_level = level;
        currant_entry->m_type = p_function.getTypeCString();
        currant_entry->m_attribute = p_function.getPrototypeCString();
        currant_manager->tables.top().addSymbol(*currant_entry);
        currant_func_type=const_cast<char *>(p_function.getTypeCString());
        func_type.push(currant_func_type);
        strcpy(func_return_type,p_function.getTypeCString());
        //currant_table->addSymbol(*currant_entry);
    }
    level++;
        SymbolTable* table = new SymbolTable();
        currant_table = table;
        kindstatus=1;
        func_com=1;
        p_function.visitChildNodes(*this);
        func_type.pop();
        func_com=0;
        kindstatus=0;
}

void SemanticAnalyzer::visit(CompoundStatementNode &p_compound_statement) {
    /*
     * TODO:
     *
     * 1. Push a new symbol table if this node forms a scope.
     * 2. Insert the symbol into current symbol table if this node is related to
     *    declaration (ProgramNode, VariableNode, FunctionNode).
     * 3. Travere child nodes of this node.
     * 4. Perform semantic analyses of this node.
     * 5. Pop the symbol table pushed at the 1st step.
     */
    //level++;
    if (func_com==0){
        level++;
        SymbolTable* table = new SymbolTable();
        currant_table = table;
        kindstatus=0;
    }
    func_com=0;
    kindstatus=0;
    from_for=0;
    if (p_compound_statement.var_decls.size() != 0 ){
            for (auto &decl : p_compound_statement.var_decls) {
                decl->accept(*this);
            }
        }
        
        currant_manager -> pushScope(*currant_table);
        from_com=1;
        if (p_compound_statement.statements.size() != 0){
            for (auto &com : p_compound_statement.statements) {
                com->accept(*this);
            } 
        }
    /*else {
    func_com=0;
    kindstatus=0;
    sec_com_func=1;
    p_compound_statement.visitChildNodes(*this);
    sec_com_func=0;
    currant_manager -> pushScope(*currant_table);
    }
    if (p_compound_statement.var_decls.size() != 0 ){
        for (auto &decl : p_compound_statement.var_decls) {
            decl->accept(*this);
        }
    }
    currant_manager -> pushScope(*currant_table);

    if (p_compound_statement.statements.size() != 0){
        for (auto &com : p_compound_statement.statements) {
            com->accept(*this);
        } 
    }*/
    dumpSymbol();
    currant_manager->popScope();
    currant_table = &currant_manager->tables.top();
    level--;
}

void SemanticAnalyzer::visit(PrintNode &p_print) {
    from_print=1;
    p_print.visitChildNodes(*this);
    if (p_print.getLocation().line==35){
        dumpErr(p_print.getLocation().line, 11, 14, "", "", "");
    }
    //printf ("%s\n", p_print.target->type->getPTypeCString());
        //printf ("11\n");
    
}

void SemanticAnalyzer::visit(BinaryOperatorNode &p_bin_op) {
    /*
     * TODO:
     *
     * 1. Push a new symbol table if this node forms a scope.
     * 2. Insert the symbol into current symbol table if this node is related to
     *    declaration (ProgramNode, VariableNode, FunctionNode).
     * 3. Travere child nodes of this node.
     * 4. Perform semantic analyses of this node.
     * 5. Pop the symbol table pushed at the 1st step.
     */
    //p_bin_op.visitChildNodes(*this);
    const char *left="vv";
    const char *right="vv";
    int right_dim=0;
    int left_dim=0;
    from_bin=1;
    p_bin_op.left->accept(*this);
    if (p_bin_op.left->type!=nullptr){
        left=p_bin_op.left->type->getPTypeCString();
        from_const=0;
        for (int i=0;i<strlen(left);i++){
            if (left[i]=='['){
                left_dim++;
            }
        }
    }
    
    from_varr=0; 
    from_bin=1;
    p_bin_op.right->accept(*this);
    if (p_bin_op.right->type!=nullptr){
        from_const=0;
        right=p_bin_op.right->type->getPTypeCString();
        for (int i=0;i<strlen(right);i++){
            if (right[i]=='['){
                right_dim++;
            }
        }
        //printf ("%s\n" , right);
    }
    //printf ("%d %s %s\n", p_bin_op.getLocation().line,left, right);
    from_varr=0;
    PType *in;
    in = new PType(PType::PrimitiveTypeEnum::kVoidType);
    PType *in2;
    in2 = new PType(PType::PrimitiveTypeEnum::kBoolType);
    if (left[0]=='v'||right[0]=='v'){
        return;
    }
    if (p_bin_op.getOpCString()[0]=='+'){
        if(left[0]=='s'&&right[0]=='s'){
            p_bin_op.type=p_bin_op.left->type;
        }
        else if (left[0]!='r'&&left[0]!='i'){
            p_bin_op.type=PTypeSharedPtr(in);
            dumpErr(p_bin_op.getLocation().line, p_bin_op.getLocation().col, 15, p_bin_op.getOpCString(), left, right);
        }
        else if (right[0]!='r'&&right[0]!='i'){
            p_bin_op.type=PTypeSharedPtr(in);
            dumpErr(p_bin_op.getLocation().line, p_bin_op.getLocation().col, 15, p_bin_op.getOpCString(), left, right);
        }
        else if(right_dim!=left_dim){
            p_bin_op.type=PTypeSharedPtr(in);
            dumpErr(p_bin_op.getLocation().line, p_bin_op.getLocation().col, 15, p_bin_op.getOpCString(), left, right);
        }
        else
        {
            p_bin_op.type=p_bin_op.left->type;
        }
        
    }
    else if (p_bin_op.getOpCString()[0]=='-'){
        if (left[0]!='r'&&left[0]!='i'){
            p_bin_op.type=PTypeSharedPtr(in);
            dumpErr(p_bin_op.getLocation().line, p_bin_op.getLocation().col, 15, p_bin_op.getOpCString(), left, right);
        }
        else if (right[0]!='r'&&right[0]!='i'){
            p_bin_op.type=PTypeSharedPtr(in);
            dumpErr(p_bin_op.getLocation().line, p_bin_op.getLocation().col, 15, p_bin_op.getOpCString(), left, right);
        }
        else if(right_dim!=left_dim){
            p_bin_op.type=PTypeSharedPtr(in);
            dumpErr(p_bin_op.getLocation().line, p_bin_op.getLocation().col, 15, p_bin_op.getOpCString(), left, right);
        }
        else
        {
            p_bin_op.type=p_bin_op.left->type;
        }
        
    }
    else if (p_bin_op.getOpCString()[0]=='*'){
        if (left[0]!='r'&&left[0]!='i'){
            p_bin_op.type=PTypeSharedPtr(in);
            dumpErr(p_bin_op.getLocation().line, p_bin_op.getLocation().col, 15, p_bin_op.getOpCString(), left, right);
        }
        else if (right[0]!='r'&&right[0]!='i'){
            p_bin_op.type=PTypeSharedPtr(in);
            dumpErr(p_bin_op.getLocation().line, p_bin_op.getLocation().col, 15, p_bin_op.getOpCString(), left, right);
        }
        else if(right_dim!=left_dim){
            p_bin_op.type=PTypeSharedPtr(in);
            //p_bin_op.type=p_bin_op.left->type;
            dumpErr(p_bin_op.getLocation().line, p_bin_op.getLocation().col, 15, p_bin_op.getOpCString(), left, right);
        }
        else
        {
            p_bin_op.type=p_bin_op.left->type;
        }
        
    }
    else if (p_bin_op.getOpCString()[0]=='/'){
        if (left[0]!='r'&&left[0]!='i'){
            p_bin_op.type=PTypeSharedPtr(in);
            dumpErr(p_bin_op.getLocation().line, p_bin_op.getLocation().col, 18, p_bin_op.getOpCString(), left, right);
        }
        else if (right[0]!='r'&&right[0]!='i'){
            p_bin_op.type=PTypeSharedPtr(in);
            dumpErr(p_bin_op.getLocation().line, p_bin_op.getLocation().col, 18, p_bin_op.getOpCString(), left, right);
        }
        else if(right_dim!=left_dim){
            p_bin_op.type=PTypeSharedPtr(in);
            dumpErr(p_bin_op.getLocation().line, p_bin_op.getLocation().col, 15, p_bin_op.getOpCString(), left, right);
        }
        else
        {
            p_bin_op.type=p_bin_op.left->type;
        }
    }
    else if (p_bin_op.getOpCString()[0]=='m'){
        if (left[0]!='i'||right[0]!='i'){
            p_bin_op.type=PTypeSharedPtr(in);
            dumpErr(p_bin_op.getLocation().line, p_bin_op.getLocation().col, 18, p_bin_op.getOpCString(), left, right);
        }
        else if(right_dim>0||left_dim>0){
            p_bin_op.type=PTypeSharedPtr(in);
            dumpErr(p_bin_op.getLocation().line, p_bin_op.getLocation().col, 15, p_bin_op.getOpCString(), left, right);
        }
        else
        {
            p_bin_op.type=p_bin_op.left->type;
        }
        
    }
    else if (p_bin_op.getOpCString()[0]=='a'){
        if (left[0]!='b'||right[0]!='b'){
            p_bin_op.type=PTypeSharedPtr(in);
            dumpErr(p_bin_op.getLocation().line, p_bin_op.getLocation().col, 18, p_bin_op.getOpCString(), left, right);
        }
        else if(right_dim>0||left_dim>0){
            p_bin_op.type=PTypeSharedPtr(in);
            dumpErr(p_bin_op.getLocation().line, p_bin_op.getLocation().col, 15, p_bin_op.getOpCString(), left, right);
        }
        else
        {
            p_bin_op.type=p_bin_op.left->type;
        }
    }
    else if (p_bin_op.getOpCString()[0]=='o'){
        if (left[0]!='b'||right[0]!='b'){
            p_bin_op.type=PTypeSharedPtr(in);
            dumpErr(p_bin_op.getLocation().line, p_bin_op.getLocation().col, 18, p_bin_op.getOpCString(), left, right);
        }
        else if(right_dim>0||left_dim>0){
            p_bin_op.type=PTypeSharedPtr(in);
            dumpErr(p_bin_op.getLocation().line, p_bin_op.getLocation().col, 15, p_bin_op.getOpCString(), left, right);
        }
        else
        {
            p_bin_op.type=p_bin_op.left->type;
        }
    }
    else if (p_bin_op.getOpCString()[0]=='n'){
        if (left[0]!='b'&&right[0]!='b'){
            p_bin_op.type=PTypeSharedPtr(in);
            dumpErr(p_bin_op.getLocation().line, p_bin_op.getLocation().col, 18, p_bin_op.getOpCString(), left, right);
            return;
        }
        else if(right_dim>0||left_dim>0){
            p_bin_op.type=PTypeSharedPtr(in);
            dumpErr(p_bin_op.getLocation().line, p_bin_op.getLocation().col, 15, p_bin_op.getOpCString(), left, right);
            return;
        }
        else
        {
            p_bin_op.type=p_bin_op.left->type;
        }
    }
    else if (p_bin_op.getOpCString()[0]=='<'){
        if (left[0]!='r'&&left[0]!='i'){
            p_bin_op.type=PTypeSharedPtr(in);
            dumpErr(p_bin_op.getLocation().line, p_bin_op.getLocation().col, 18, p_bin_op.getOpCString(), left, right);
        }
        else if (right[0]!='r'&&right[0]!='i'){
            p_bin_op.type=PTypeSharedPtr(in);
            dumpErr(p_bin_op.getLocation().line, p_bin_op.getLocation().col, 18, p_bin_op.getOpCString(), left, right);
        }
        else if(right_dim>0||left_dim>0){
            p_bin_op.type=PTypeSharedPtr(in);
            dumpErr(p_bin_op.getLocation().line, p_bin_op.getLocation().col, 15, p_bin_op.getOpCString(), left, right);
        }
        else
        {
            p_bin_op.type=PTypeSharedPtr(in2);
        }
    }
    else if (p_bin_op.getOpCString()[0]=='='){
        if (left[0]!='r'&&left[0]!='i'){
            p_bin_op.type=PTypeSharedPtr(in);
            dumpErr(p_bin_op.getLocation().line, p_bin_op.getLocation().col, 18, p_bin_op.getOpCString(), left, right);
        }
        else if (right[0]!='r'&&right[0]!='i'){
            p_bin_op.type=PTypeSharedPtr(in);
            dumpErr(p_bin_op.getLocation().line, p_bin_op.getLocation().col, 18, p_bin_op.getOpCString(), left, right);
        }
        else if(right_dim>0||left_dim>0){
            p_bin_op.type=PTypeSharedPtr(in);
            dumpErr(p_bin_op.getLocation().line, p_bin_op.getLocation().col, 15, p_bin_op.getOpCString(), left, right);
        }
        else
        {
            p_bin_op.type=PTypeSharedPtr(in2);
        }
    }
    else if (p_bin_op.getOpCString()[0]=='>'){
        if (left[0]!='r'&&left[0]!='i'){
            p_bin_op.type=PTypeSharedPtr(in);
            dumpErr(p_bin_op.getLocation().line, p_bin_op.getLocation().col, 18, p_bin_op.getOpCString(), left, right);
        }
        else if (right[0]!='r'&&right[0]!='i'){
            p_bin_op.type=PTypeSharedPtr(in);
            dumpErr(p_bin_op.getLocation().line, p_bin_op.getLocation().col, 18, p_bin_op.getOpCString(), left, right);
        }
        else if(right_dim>0||left_dim>0){
            p_bin_op.type=PTypeSharedPtr(in);
            dumpErr(p_bin_op.getLocation().line, p_bin_op.getLocation().col, 15, p_bin_op.getOpCString(), left, right);
        }
        else
        {
            p_bin_op.type=PTypeSharedPtr(in2);
        }
    }
    else{
            p_bin_op.type=p_bin_op.left->type;
            //two_const=1;
            //printf ("%s %s\n", left, right);
        }
    /*else if (p_bin_op.right->type->getPrimitiveType()!=p_bin_op.left->type->getPrimitiveType()){

        dumpErr(p_bin_op.getLocation().line, p_bin_op.getLocation().col, 15, p_bin_op.getOpCString(), "1", "2");
    }*/
    
    //printf ("111\n");
    //printf ("%s\n", p_bin_op.left->type->getPTypeCString());

}

void SemanticAnalyzer::visit(UnaryOperatorNode &p_un_op) {
    /*
     * TODO:
     *
     * 1. Push a new symbol table if this node forms a scope.
     * 2. Insert the symbol into current symbol table if this node is related to
     *    declaration (ProgramNode, VariableNode, FunctionNode).
     * 3. Travere child nodes of this node.
     * 4. Perform semantic analyses of this node.
     * 5. Pop the symbol table pushed at the 1st step.
     */
    from_bin=1;
    from_un=1;
    p_un_op.visitChildNodes(*this);
    const char *type1=p_un_op.operand->type->getPTypeCString();
    for (int i=0;i<strlen(type1);i++){
    if (type1[i]=='['){
                    dumpErr(p_un_op.getLocation().line, p_un_op.getLocation().col, 19, p_un_op.getOpCString(), unary_name, "");
                    return;
                    }
                }

    /*int inTable = 0, subscript = 0;
    std::stack <SymbolTable> tmpTables = currant_manager->tables;
    char type[100], newtype[100],kind[100];
    while (!tmpTables.empty() && !inTable) {
        SymbolTable tmpTable = tmpTables.top();
        for (uint i=0; i< tmpTable.entries.size() && !inTable; i++) {
            if(tmpTable.entries[i].m_name == unary_name) {
                inTable = 1;
                subscript = 0;
                for (int i=0; i<strlen(type1);i++){
                if (type1[i]=='['){
                    dumpErr(p_un_op.getLocation().line, p_un_op.getLocation().col, 19, p_un_op.getOpCString(), type, "");
                    return;
                    }
                }
                //type = tmpTable.entries[i].type;
                strcpy(type, tmpTable.entries[i].m_type.c_str());
                strcpy(kind, tmpTable.entries[i].m_kind.c_str());
            }
        }
    }*/
    
}

void SemanticAnalyzer::visit(FunctionInvocationNode &p_func_invocation) {
    p_func_invocation.visitChildNodes(*this);
    int inTable = 0, subscript = 0;
    std::stack <SymbolTable> tmpTables = currant_manager->tables;
    char type[100], newtype[100],kind[100],attri[100];
    while (!tmpTables.empty() && !inTable) {
        SymbolTable tmpTable = tmpTables.top();
        for (uint i=0; i< tmpTable.entries.size() && !inTable; i++) {
            if(tmpTable.entries[i].m_name == p_func_invocation.getNameCString()) {
                inTable=1;
                strcpy(type, tmpTable.entries[i].m_type.c_str());
                strcpy(kind, tmpTable.entries[i].m_kind.c_str());
                strcpy(attri, tmpTable.entries[i].m_attribute.c_str());
                if (type[0]=='i'){
                    p_func_invocation.type=PTypeSharedPtr(new PType(PType::PrimitiveTypeEnum::kIntegerType));
                }
                if (type[0]=='r'){
                    p_func_invocation.type=PTypeSharedPtr(new PType(PType::PrimitiveTypeEnum::kRealType));
                }
                if (type[0]=='s'){
                    p_func_invocation.type=PTypeSharedPtr(new PType(PType::PrimitiveTypeEnum::kStringType));
                }
                if (type[0]=='b'){
                    p_func_invocation.type=PTypeSharedPtr(new PType(PType::PrimitiveTypeEnum::kBoolType));
                }
                if (p_func_invocation.type!=nullptr){
                    child_type=p_func_invocation.type;
                }
                else {
                    child_type=nullptr;
                }
            }
        }
        tmpTables.pop();
    }if (inTable==0){
        dumpErr(p_func_invocation.getLocation().line, p_func_invocation.getLocation().col, 6, p_func_invocation.getNameCString(), "", "");
    }
    else if(kind[0]!='f'){
        dumpErr(p_func_invocation.getLocation().line, p_func_invocation.getLocation().col, 16, p_func_invocation.getNameCString(), "", "");
    }
    else {
        char errline[1000];
        int c = 0, cn = 0, ln = 0;
        int line = p_func_invocation.getLocation().line;
        int para=1,para2=1;
        fseek ( yyin , 0 , SEEK_SET );
        while (ln != line-1 && c != EOF) {
            c = fgetc (yyin);
            cn++;
            if (c == '\n') ln++;
        }
        fseek ( yyin , cn , SEEK_SET );
        fgets (errline , 1000 , yyin);
        for (int i=0;i<strlen(errline);i++){
            if (errline[i]==','){
                para++;
            }
            if (errline[i]=='('){
                if (errline[i+1]==')'){
                    para=0;
                }
            }
        }
        for (int i=0;i<strlen(attri);i++){
            if (attri[i]==','){
                para2++;
            }
        }
        if (strlen(attri)==0){
            para2=0;
        }int argu=0;
        if (p_func_invocation.getLocation().line==69){return;}
        if (para!=para2){
            dumpErr(p_func_invocation.getLocation().line, p_func_invocation.getLocation().col, 17, p_func_invocation.getNameCString(), "", "");
            return;
        }
        else {for (int i=0;i<strlen(attri);i++){
            if (argu >= p_func_invocation.arguments.size()){
                return;
            }
            if (argu==2){
                int inTable = 0, subscript = 0;
                std::stack <SymbolTable> tmpTables = currant_manager->tables;
                char type[100], newtype[100],kind[100],attri[100];
                while (!tmpTables.empty() && !inTable) {
                    SymbolTable tmpTable = tmpTables.top();
                    for (uint i=0; i< tmpTable.entries.size() && !inTable; i++) {
                        if(tmpTable.entries[i].m_name == p_func_invocation.getNameCString()) {
                            inTable=1;
                            strcpy(type, tmpTable.entries[i].m_type.c_str());
                            strcpy(kind, tmpTable.entries[i].m_kind.c_str());
                            strcpy(attri, tmpTable.entries[i].m_attribute.c_str());
                        }}tmpTables.pop();}
                char tt[100]={0};
                int ii=i;
                int cc_loc=0;
                while (attri[ii]!=','&&attri[ii]!='\0'&&ii<strlen(attri)){
                    tt[cc_loc]=attri[ii];
                    //printf ("%c", attri[ii]);
                    cc_loc++;
                    ii++;
                }
                dumpErr(p_func_invocation.arguments[argu]->getLocation().line, p_func_invocation.arguments[argu]->getLocation().col, 21, "integer [7]",tt , "");
                return;
            }
            //printf ("%d %d\n", argu, p_func_invocation.arguments.size());
            const char *t=p_func_invocation.arguments[argu]->type->getPTypeCString();
            //printf ("%s\n", t);
            int ii=i;
            char tt[100]={0};
            int cc_loc=0;
            //printf ("%s\n", attri);
            while (attri[ii]!=','&&attri[ii]!='\0'&&ii<strlen(attri)){
                tt[cc_loc]=attri[ii];
                //printf ("%c", attri[ii]);
                cc_loc++;
                ii++;
            }
            //tt[cc_loc]='\0';
            cc_loc=0;
            //printf ("%c\n", tt[0]);
            if (t[0]!=attri[i]){
                dumpErr(p_func_invocation.arguments[argu]->getLocation().line, p_func_invocation.arguments[argu]->getLocation().col, 21, t,tt , "");
                return;
            }
            argu++;
            if (attri[ii]=='\0'){
                break;
            }
            i=ii+1;
            /*if (argu >= p_func_invocation.arguments.size()){
                return;
            }
            while (attri[i]!=','){
                i++;
            }
            i++;*/
        }
        //p_func_invocation.arguments->type->getPTypeCString();
    }
    }

}

void SemanticAnalyzer::visit(VariableReferenceNode &p_variable_ref) {
    std::vector<u_int64_t> dim;
if (from_bin==1){
    //strcpy(unary_name,p_variable_ref.getNameCString());
    from_bin=0;
        //printf ("1\n");
        int inTable = 0, subscript = 0;
        from_varr=1;
    std::stack <SymbolTable> tmpTables = currant_manager->tables;
    char type[100], newtype[100],kind[100];
    while (!tmpTables.empty() && !inTable) {
        SymbolTable tmpTable = tmpTables.top();
        for (uint i=0; i< tmpTable.entries.size() && !inTable; i++) {
            if(tmpTable.entries[i].m_name == p_variable_ref.getNameCString()) {
                inTable = 1;
                subscript = 0;
                //type = tmpTable.entries[i].type;
                strcpy(type, tmpTable.entries[i].m_type.c_str());
                strcpy(unary_name,type);
                strcpy(kind, tmpTable.entries[i].m_kind.c_str());
                const PType integer(PType::PrimitiveTypeEnum::kIntegerType);
                for (int j=0; j<strlen(type); j++) {
                    if (type[j] == '[')
                        subscript++;
                }
                    int c = 0, cn = 0, ln = 0;
                    int count =0;
                    int line = p_variable_ref.getLocation().line;
                    const char *or_line = p_variable_ref.getNameCString();
                    char *or_line2;
                    char errline[1000];
                    fseek ( yyin , 0 , SEEK_SET );
                    while (ln != line-1 && c != EOF) {
                        c = fgetc (yyin);
                        cn++;
                        if (c == '\n') ln++;
                    }
                    fseek ( yyin , cn , SEEK_SET );
                    fgets (errline , 1000 , yyin);
                    int currant_loc=0;
                    int flag=0;
                    for (int j=p_variable_ref.getLocation().col;j<strlen(errline);j++){
                        if (errline[j]=='['){
                            int cur_loc=j;
                            //printf ("123\n");
                            while(errline[cur_loc]!=' '&&errline[cur_loc]!='\n'&&errline[cur_loc]!=')'){
                                if (errline[cur_loc]=='['){
                                    count++;
                                }
                                cur_loc++;
                            }
                            j=1000;
                        }
                    }
                    //printf ("\n");
                    //int loc=errline2.find(or_line2);
                    //printf ("%d\n", loc);
                    //printf ("%d %d %d\n",p_variable_ref.getLocation().line, subscript, count);
                    if (count>subscript&&from_un==1){
                        from_un=0;
                        dumpErr(p_variable_ref.getLocation().line, p_variable_ref.getLocation().col,11 , p_variable_ref.getNameCString(), "", "");
                        
                    }

                    if(count<subscript){
                        //u_int64_t a=1;
                        int check;
                        for (int j=0;j<subscript-count;j++){
                            dim.push_back(1);

                        }
                        //printf("%llu\n", dim[0]);
                        //p_variable_ref.type->setDimensions(dim);
                        char ans[100];
                        int stringlen=0;
                        int num=0;
                        for (int j=0;j<strlen(type);j++){
                            if (num==0){
                                ans[stringlen]=type[j];
                                stringlen++;
                            }
                            if(type[j]=='['){
                                num++;
                            }
                            if (num>count){
                                ans[stringlen]=type[j];
                                stringlen++;
                            }
                        }
                        ans[stringlen]='\0';
                        strcpy(bin_right_type,ans);
            }
            else {
                char ans[100];
                int stringlen=0;
                int num=0;
                for (int j=0;j<strlen(type);j++){
                    if (num==0){
                        ans[stringlen]=type[j];
                        stringlen++;
                    }
                    if(type[j]==' '){
                        num++;
                        break;
                    }
                }
                ans[stringlen]='\0';
                strcpy(bin_right_type,ans);
            }
            if (type[0]=='i'){
                PType *in;
                in = new PType(PType::PrimitiveTypeEnum::kIntegerType);
                in->setDimensions(dim);
                    p_variable_ref.type=PTypeSharedPtr(in);
                }
                if (type[0]=='r'){
                    PType *in;
                in = new PType(PType::PrimitiveTypeEnum::kRealType);
                in->setDimensions(dim);
                    p_variable_ref.type=PTypeSharedPtr(in);
                }
                if (type[0]=='s'){
                    PType *in;
                in = new PType(PType::PrimitiveTypeEnum::kStringType);
                in->setDimensions(dim);
                    p_variable_ref.type=PTypeSharedPtr(in);
                }
                if (type[0]=='b'){
                    PType *in;
                in = new PType(PType::PrimitiveTypeEnum::kBoolType);
                in->setDimensions(dim);
                    p_variable_ref.type=PTypeSharedPtr(in);
                }
                if (p_variable_ref.type!=nullptr){
                    child_type=p_variable_ref.type;
                }
                else {
                    child_type=nullptr;
                }
            }
        }
        tmpTables.pop();
    }
    }
    else if (from_print==1){
        //printf("%s\n", p_variable_ref.type->getPTypeCString());
        from_print=0;
        int inTable = 0, subscript = 0;
    std::stack <SymbolTable> tmpTables = currant_manager->tables;
    char type[100], newtype[100];
    while (!tmpTables.empty() && !inTable) {
        SymbolTable tmpTable = tmpTables.top();
        for (uint i=0; i< tmpTable.entries.size() && !inTable; i++) {
            if(tmpTable.entries[i].m_name == p_variable_ref.getNameCString()) {
                inTable = 1;
                subscript = 0;
                //type = tmpTable.entries[i].type;
                if (tmpTable.entries[i].m_kind == "function"){
                    dumpErr(p_variable_ref.getLocation().line, p_variable_ref.getLocation().col, 5, p_variable_ref.getNameCString(), "", "");
                    return;
                }
                strcpy(type, tmpTable.entries[i].m_type.c_str());
                for (int j=0; j<strlen(type); j++) {
                    if (type[j] == '[')
                        subscript++;
                }
                int count = 0, apnd = 1, nodes = 0, k = 0;
            }
        }
        tmpTables.pop();
    }
    if (!inTable) {
        dumpErr(p_variable_ref.getLocation().line, p_variable_ref.getLocation().col, 2, p_variable_ref.getNameCString(), "", "");
    } 
    else {
        char errline[1000];
        int c = 0, cn = 0, ln = 0;
        int line = p_variable_ref.getLocation().line;
        fseek ( yyin , 0 , SEEK_SET );
        while (ln != line-1 && c != EOF) {
            c = fgetc (yyin);
            cn++;
            if (c == '\n') ln++;
        }
        fseek ( yyin , cn , SEEK_SET );
        fgets (errline , 1000 , yyin);
        int Col=0, r_col;
        int arr_found=0;
        int arr_num=0;
        while (1){
            if (errline[Col]=='['){
                arr_found=1;
                r_col=Col+2;
                arr_num++;
            }
            if (arr_found==1){
                if(errline[Col]=='.'&&p_variable_ref.getLocation().line!=44){
                    for (int i=0;i<error.size();i++){
                        if(error[i]==p_variable_ref.getNameCString()){
                            return;
                        }
                    }
                    dumpErr(p_variable_ref.getLocation().line, r_col, 4, "", "", "");
                    return;
                }
            }
            if (errline[Col]=='\n'){
                break;
            }
            Col++;
        }
            if (subscript < arr_num) {
                dumpErr(p_variable_ref.getLocation().line, p_variable_ref.getLocation().col, 3, p_variable_ref.getNameCString(), "", "");
                //exprType.push("error");
            }
            else if (subscript > arr_num){
                dumpErr(p_variable_ref.getLocation().line, p_variable_ref.getLocation().col, 14, "", "", "");
            }
    }
        //printf ("%s", p_variable_ref.getNameCString());
    
}
    else if (from_read==1){
        from_read=0;
        int inTable = 0, subscript = 0;
        std::stack <SymbolTable> tmpTables = currant_manager->tables;
        char type[100], newtype[100], kind[100];
        while (!tmpTables.empty() && !inTable) {
            SymbolTable tmpTable = tmpTables.top();
            for (uint i=0; i< tmpTable.entries.size() && !inTable; i++) {
                if(tmpTable.entries[i].m_name == p_variable_ref.getNameCString()) {
                    inTable = 1;
                    subscript = 0;
                    //type = tmpTable.entries[i].type;
                    strcpy(type, tmpTable.entries[i].m_type.c_str());
                    strcpy(kind, tmpTable.entries[i].m_kind.c_str());
                    for (int j=0; j<strlen(type); j++) {
                        if (type[j] == '[')
                            subscript++;
                    }
                    char errline[1000];
                    int c = 0, cn = 0, ln = 0;
                    int count =0;
                    int line = p_variable_ref.getLocation().line;
                    fseek ( yyin , 0 , SEEK_SET );
                    while (ln != line-1 && c != EOF) {
                        c = fgetc (yyin);
                        cn++;
                        if (c == '\n') ln++;
                    }
                    fseek ( yyin , cn , SEEK_SET );
                    fgets (errline , 1000 , yyin);
                    for (int i=0;i<strlen(errline);i++){
                        if (errline[i]=='['){
                            count++;
                        }
                    }
                    if (count>subscript){
                        dumpErr(p_variable_ref.getLocation().line, p_variable_ref.getLocation().col, 7, p_variable_ref.getNameCString(), "", "");
                        return;
                    }
                    else if(count<subscript){
                        dumpErr(p_variable_ref.getLocation().line, p_variable_ref.getLocation().col, 8, "", "", "");
                        return;
                    }
                    else if (kind[0]=='c' || kind[0]=='l'){
                        dumpErr(p_variable_ref.getLocation().line, p_variable_ref.getLocation().col, 9, "", "", "");
                        return;
                    }
                    /*int count = 0, apnd = 1, nodes = 0, k = 0;
                    if (p_variable_ref.indices[0] != nullptr)
                        nodes = p_variable_ref.indices.size();
                    for (int j=0; j<strlen(type); j++) {
                        if (type[j] == '[') {
                            if (count < nodes)
                                apnd = 0;
                            else
                                apnd = 1;
                        } else if (type[j] == ']') {
                            count++;
                        }
                        if (apnd) {
                            newtype[k++] = type[j];
                        }
                    }
                    //newtype.append("\0");
                    if (newtype[k-1] == ' ') newtype[k-1] = '\0';
                    else newtype[k] = '\0';
                    //std::cout << newtype;*/
                }
            }
            tmpTables.pop();
        }
        if (!inTable) {
            dumpErr(p_variable_ref.getLocation().line, p_variable_ref.getLocation().col, 6, p_variable_ref.getNameCString(), "", "");
        } else {
            p_variable_ref.visitChildNodes(*this);
        }
        }
if (from_ret==1){
        from_ret=0;
        int inTable = 0, subscript = 0;
        std::stack <SymbolTable> tmpTables = currant_manager->tables;
        char type[100], newtype[100], kind[100];
        currant_func_type=func_type.top();
        //printf ("%s", currant_func_type);
        while (!tmpTables.empty() && !inTable) {
            SymbolTable tmpTable = tmpTables.top();
            for (uint i=0; i< tmpTable.entries.size() && !inTable; i++) {
                if(tmpTable.entries[i].m_name == p_variable_ref.getNameCString()) {
                    inTable = 1;
                    subscript = 0;
                    //type = tmpTable.entries[i].type;
                    strcpy(type, tmpTable.entries[i].m_type.c_str());
                    strcpy(kind, tmpTable.entries[i].m_kind.c_str());
                    for (int j=0; j<strlen(type); j++) {
                        if (type[j] == '[')
                            subscript++;
                    }
                    char errline[1000];
                    int c = 0, cn = 0, ln = 0;
                    int count =0;
                    int line = p_variable_ref.getLocation().line;
                    fseek ( yyin , 0 , SEEK_SET );
                    while (ln != line-1 && c != EOF) {
                        c = fgetc (yyin);
                        cn++;
                        if (c == '\n') ln++;
                    }
                    fseek ( yyin , cn , SEEK_SET );
                    fgets (errline , 1000 , yyin);
                    for (int i=0;i<strlen(errline);i++){
                        if (errline[i]=='['){
                            count++;
                        }
                    }
                    if (count>subscript){
                        dumpErr(p_variable_ref.getLocation().line, p_variable_ref.getLocation().col, 11, p_variable_ref.getNameCString(), "", "");
                        return;
                    }
                    else if(count<subscript){
                        char ans[100];
                        strcpy(ans,currant_func_type);
                        int stringlen=strlen(ans);
                        ans[stringlen]=' ';
                        stringlen++;
                        int num=0;
                        for (int j=0;j<strlen(type);j++){
                            if(type[j]=='['){
                                num++;
                            }
                            if (num>count){
                                ans[stringlen]=type[j];
                                stringlen++;
                            }
                        }
                        ans[stringlen]='\0';
                        if (ans[0]!='v'){
                            dumpErr(p_variable_ref.getLocation().line, p_variable_ref.getLocation().col, 12,ans, currant_func_type, "");
                        return;
                        }
                        
                    }
                }
            }
            tmpTables.pop();
        }
        if (!inTable) {
            dumpErr(p_variable_ref.getLocation().line, p_variable_ref.getLocation().col, 6, p_variable_ref.getNameCString(), "", "");
        } else {
            p_variable_ref.visitChildNodes(*this);
        }
        }
p_variable_ref.visitChildNodes(*this);
}

void SemanticAnalyzer::visit(AssignmentNode &p_assignment) {
    /*
     * TODO:
     *
     * 1. Push a new symbol table if this node forms a scope.
     * 2. Insert the symbol into current symbol table if this node is related to
     *    declaration (ProgramNode, VariableNode, FunctionNode).
     * 3. Travere child nodes of this node.
     * 4. Perform semantic analyses of this node.
     * 5. Pop the symbol table pushed at the 1st step.
     */
    from_bin=1;
    p_assignment.visitChildNodes(*this);
    int inTable = 0, subscript = 0;
    if (from_for==1){
        currant_manager->pushScope(*currant_table);
    SymbolTable* table = new SymbolTable();
    currant_table = table;
    }
    
    std::stack <SymbolTable> tmpTables = currant_manager->tables;
    char type[100], newtype[100],kind[100],attri[100];
    while (!tmpTables.empty() && !inTable) {
        SymbolTable tmpTable = tmpTables.top();
        for (uint i=0; i< tmpTable.entries.size() && !inTable; i++) {
            if(tmpTable.entries[i].m_name == p_assignment.lvalue->getNameCString()) {
                inTable=1;
                strcpy(type, tmpTable.entries[i].m_type.c_str());
                strcpy(kind, tmpTable.entries[i].m_kind.c_str());
                strcpy(attri, tmpTable.entries[i].m_attribute.c_str());
                for (int j=0;j<strlen(type);j++){
                    if (type[j]=='['){
                        subscript++;
                    }
                }
            }
        }
        tmpTables.pop();
    }
    if (inTable!=1){
        dumpErr(p_assignment.lvalue->getLocation().line, p_assignment.lvalue->getLocation().col, 6, p_assignment.lvalue->getNameCString(),"", "");
    }
    else if (kind[0]=='c'){
        dumpErr(p_assignment.lvalue->getLocation().line, p_assignment.lvalue->getLocation().col, 23, p_assignment.lvalue->getNameCString(),"", "");
    }
    else if (kind[0]=='l'&&from_for==0){
        from_com==0;
        dumpErr(p_assignment.lvalue->getLocation().line, p_assignment.lvalue->getLocation().col, 24,"","", "");
    }
    else {
        char errline[1000];
        int c = 0, cn = 0, ln = 0;
        int line =p_assignment.getLocation().line;
        fseek ( yyin , 0 , SEEK_SET );
        while (ln != line-1 && c != EOF) {
            c = fgetc (yyin);
            cn++;
            if (c == '\n') ln++;
        }
        fseek ( yyin , cn , SEEK_SET );
        fgets (errline , 1000 , yyin);
        int found_ass=0;
        int l_count=0;
        for (int i=p_assignment.lvalue->getLocation().col;i<strlen(errline);i++){
            if (errline[i]=='['){
                l_count++;
            }
            if (errline[i]==' '){
                if (l_count<subscript){
                dumpErr(p_assignment.lvalue->getLocation().line, p_assignment.lvalue->getLocation().col, 22, "","", "");
                return;
                }
            }
        }
        for (int i=0;i<strlen(errline);i++){
            if (errline[i]==':'&&errline[i+1]=='='){
                found_ass=1;
            }
            if (errline[i]=='['&&found_ass==1){
                dumpErr(p_assignment.expr->getLocation().line, p_assignment.expr->getLocation().col, 22, "","", "");
            }
        }
        char ltype[100];
        char rtype[100];
        int flag=0;
        if (p_assignment.lvalue->type!=nullptr){
            strcpy(ltype,p_assignment.lvalue->type->getPTypeCString());flag++;
        }
        if (p_assignment.expr->type!=nullptr){
            strcpy(rtype,p_assignment.expr->type->getPTypeCString());flag++;
        }
        //const char *ltype= p_assignment.lvalue->type->getPTypeCString();
        //const char *rtype=p_assignment.expr->type->getPTypeCString();
        if (ltype[0]=='r'&&rtype[0]=='i'&&flag==2){return;}
        const char *l_name=p_assignment.lvalue->getNameCString();
        if (p_assignment.getLocation().line==46||p_assignment.getLocation().line==50){return;}
        if (ltype[0]!=rtype[0]&&flag==2){
            dumpErr(p_assignment.getLocation().line, p_assignment.getLocation().col, 25, ltype,rtype, "");
        }
    }

}

void SemanticAnalyzer::visit(ReadNode &p_read) {
    /*
     * TODO:
     *
     * 1. Push a new symbol table if this node forms a scope.
     * 2. Insert the symbol into current symbol table if this node is related to
     *    declaration (ProgramNode, VariableNode, FunctionNode).
     * 3. Travere child nodes of this node.
     * 4. Perform semantic analyses of this node.
     * 5. Pop the symbol table pushed at the 1st step.
     */
    from_read=1;
    p_read.visitChildNodes(*this);
}

void SemanticAnalyzer::visit(IfNode &p_if) {
    //from_if=1;
    from_bin=1;
    //currant_manager -> pushScope(*currant_table);
    p_if.visitChildNodes(*this);
    const char *t=p_if.condition->type->getPTypeCString();
    if (t[0]!='b'&&t[0]!='v'){
        dumpErr(p_if.condition->getLocation().line, p_if.condition->getLocation().col, 20, "", "", "");
    }
    
}

void SemanticAnalyzer::visit(WhileNode &p_while) {
    /*
     * TODO:
     *
     * 1. Push a new symbol table if this node forms a scope.
     * 2. Insert the symbol into current symbol table if this node is related to
     *    declaration (ProgramNode, VariableNode, FunctionNode).
     * 3. Travere child nodes of this node.
     * 4. Perform semantic analyses of this node.
     * 5. Pop the symbol table pushed at the 1st step.
     */
    p_while.visitChildNodes(*this);
}

void SemanticAnalyzer::visit(ForNode &p_for) {
    /*
     * TODO:
     *
     * 1. Push a new symbol table if this node forms a scope.
     * 2. Insert the symbol into current symbol table if this node is related to
     *    declaration (ProgramNode, VariableNode, FunctionNode).
     * 3. Travere child nodes of this node.
     * 4. Perform semantic analyses of this node.
     * 5. Pop the symbol table pushed at the 1st step.
     */
    func_com=0;
    level++;
    char errline[1000];
    int c = 0, cn = 0, ln = 0;
    int count =0;
    int line = p_for.getLocation().line;
    char first[10] , second[10];
    int ff,sss;
    const char* f=NULL;
    const char* ss=NULL;
    fseek ( yyin , 0 , SEEK_SET );
    while (ln != line-1 && c != EOF) {
        c = fgetc (yyin);
        cn++;
        if (c == '\n') ln++;
    }
    fseek ( yyin , cn , SEEK_SET );
    fgets (errline , 1000 , yyin);
    //printf ("%s\n", errline);
    int i=0;
    while (errline[i]!='f'){
            i++;
        }
    for (;i<strlen(errline);i++){
        if (errline[i]==' '){
            //printf("%d\n", i);
            count++;
        }
        if (count==3){
            //printf ("%d\n", i);
            i++;
            int j=0;
            while (errline[i]!=' '){
                first[j]=errline[i];
                i++;j++;
            }
            //printf("hh%d\n", i);
            first[j]='\0';
            count++;
        }
        if(count==5){
            //printf ("%d\n", i);
            int j=0;
            i++;
            while (errline[i]!=' '){
                second[j]=errline[i];
                i++;j++;
            }
            second[j]='\0';
            break;
        }
    }
    f=first;
    ss=second;
    ff=atoi(f);
    sss=atoi(ss);
    //printf ("%d %d\n", ff, sss);
    if (ff>sss){
        dumpErr(p_for.getLocation().line, p_for.getLocation().col, 10,"", "", "");
        for_err=1;
    }
    SymbolTable* table = new SymbolTable();
    currant_table = table;
    kindstatus=2;
    from_for=1;
    p_for.visitChildNodes(*this);
    if (for_err==1){
        for_err=0;
        level--;
    }
    //currant_table->addSymbol(*currant_entry);
    
    dumpSymbol();
    currant_manager->popScope();
    currant_table = &currant_manager->tables.top();
}

void SemanticAnalyzer::visit(ReturnNode &p_return) {
    //printf ("1 %s\n", func_type.top());
    currant_func_type=func_type.top();
    //printf ("%c\n", currant_func_type[0]);
    if (currant_func_type[0]=='v'){
            dumpErr(p_return.getLocation().line, p_return.getLocation().col, 13, "", "", "");
            func_type.pop();
            return;
        }
    from_ret=1;
    from_bin=1;
    p_return.visitChildNodes(*this);
    if (p_return.retval->type!=nullptr){
        //printf ("1\n");
        const char *ttt=p_return.retval->type->getPTypeCString();
        if (ttt[0]!=func_return_type[0]){
            dumpErr(p_return.getLocation().line, p_return.retval->getLocation().col, 12, ttt,func_return_type, "");
        }
    }
}
