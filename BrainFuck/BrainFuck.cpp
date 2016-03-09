
 #include <string>
 #include <iostream>
 #include <fstream>
 #include <limits> 
 #include <map>

 using namespace std;

 typedef string bf_string;
 typedef const char token_type;
 typedef unsigned int circular_times_type;
 typedef unsigned int bf_int;

 token_type circular_left = '[';
 token_type circular_right = ']';
 token_type less_than = '<';
 token_type greater_than = '>';
 token_type reduce = '-';
 token_type add = '+';
 token_type output = '.';
 token_type input = ',';

 const bf_int  ram_max = 1024;


 /*
 Hello World!
 ++++++++++[>+++++++>++++++++++>+++>+<<<<-]>++.>+.+++++++..+++.>++.<<+++++++++++++++.>.+++.------.--------.>+.>.
 */
 /*
 circular times test
 +++++++++++++++++++.[+++++-------]
 */




 int main(int argc, char* argv[])
 {
      cout << "BrainFuck interpreter by karakapi" << endl << endl;
      cout << ">      means     ++p; " << endl;
      cout << "<      means     --p;" << endl;
      cout << "+      means     ++*p;" << endl;
      cout << "-      means      --*p;" << endl;
      cout << ".     means      putchar(*p);" << endl;
      cout << ",      means      *p = getchar();" << endl;
      cout << "[     means      while (*p) { " << endl;
      cout << "]     means      }" << endl;
      cout  << "The memory is persistence" << endl;
      cout << "Especially,this interpreter  provide operators" << endl;

      cout << "~ does clear memory" << endl;
      cout << "! does forcibly stop running when it  is inputting" << endl;
      cout << "@ does change the limit circular times " << endl;
      cout << "# as a key does close this interpreter " << endl;
      cout << "% does show the pointer" << endl;

      cout << "$ is a tag that is to open the function of inputting character directly" << endl
           << " You can input the ASCII" << endl << " to the location of the  pointer is pointing to  directly" << endl
           << "For example," << endl
           << "$" << endl
           << "a." << endl
           << "when you press enter, it will store the last one and show it" << endl
           << "a" << endl
           << "and when you do not input $,it is closed," << endl << "when you input $,it opens until you input $ again." << endl << endl
           << "If you want to input file to interprete ,you can command :" << endl
           << "brainfuck file " << endl << endl
           << "Pay attention to" << endl
           << "when pointer equals 1023,you use >,it will be change to 0" << endl
           << "when pointer equals 0,you use <,it will be change to 1023" << endl << endl
     
           << "Hello World  in BrainFuck" << endl
           << "++++++++++[>+++++++>++++++++++>+++>+<<<<-]" << endl
           << ">++.>+.+++++++..+++.>++.<<+++++++++++++++." << endl
           << ">.++ + .------.--------.>+.>." << endl;

      int interpreter(bf_string &token, char* array_ram, int ram_size, bool&  s, int& pointer, bf_int& limit_circular_times);

      bf_string token;
      char*array_ram = new char[ram_max + 1];
      if (array_ram == 0){
           cout << "Error cannot storage allocation" << endl;
           return false;
      }
      memset(array_ram, 0, (ram_max+1)*sizeof(char));
      int state = 0;
      bool sp = false;
      int pointer = 0;
      circular_times_type limit_circular_times = 65534;

      if (argc < 2){

      }
      else{
          
           ifstream file(argv[1]);
           while (!file.eof()){
                file >> token;

           }
           file.close();
           state = interpreter(token, array_ram,ram_max,  sp, pointer, limit_circular_times);
      }

      while (state < 2){

           cout << endl << ">> ";
           token.clear();
           cin >> token;

           state = interpreter(token, array_ram, ram_max,  sp, pointer, limit_circular_times);


      }

      delete[]array_ram;

      return 0;
 }

 int interpreter(bf_string &token, char * array_ram, int ram_size, bool& s, int&pointer, bf_int &limit_circular_times){
      int  circular_layers = 0;

      int fix_ram_size = ram_size - 1;

      map<bf_int, circular_times_type> circular_map;

     


      /*Loop syntax check*/
      bf_int index = 0;
      for (; index < token.length(); ++index){
           if (token[index] == circular_left){
                ++circular_layers;
           }
           if (token[index] == circular_right){
                if (circular_layers > 0){
                     --circular_layers;
                }
                else{
                     cout << "Error:Expect token '[' but not found" << " possible position:" << index << endl;
                     return false;
                }
           }
      }
      if (circular_layers != 0){
           if (circular_layers <0){


                for (; circular_layers<0; ++circular_layers){
                     for (; index>0; --index){
                          if (token[index] == circular_left){

                               break;

                          }

                     }
                     ++index;
                }
                cout << "Error:Expect token '[' but not found," << " possible position:" << index << endl;
                return false;
           }
           else {

                for (; circular_layers>0; --circular_layers){
                     for (; index > 0; --index){
                    
                          if (token[index] == circular_left){
                               break;
                          }
                     }
                     ++index;
                }

                cout << "Error:Expect token ']' but not found," << " possible position:" << index << endl;
                return false;

           }
      }
      /*running*/
      for (bf_int index = 0; index < token.length(); ++index){


           switch (token[index])
           {
           case circular_left:{
                                       if (pointer>fix_ram_size || pointer<0){
                                            cout << "Error:" << "circular_left" << "    " << "pointer:" << pointer << " " << "position:" << index << endl;

                                            return false;
                                       }
                                   
                                      
                                       circular_layers = 1;

                                       if (array_ram[pointer] == 0){
                                            break;
                                       }

                                       while (circular_layers){
                                            ++index;
                                            if (circular_left == token[index])
                                            {
                                                 ++circular_layers;
                                            }
                                            if (circular_right == token[index])
                                            {
                                                 --circular_layers;
                                            }

                                       }


                                       --index;
                                       break;

           }
           case circular_right:{
                                         if (pointer > fix_ram_size || pointer<0){
                                              cout << "Error:" << "circular_right" << "    " << "pointer:" << pointer << " " << "position:" << index << endl;

                                              return false;
                                         }
                                         circular_times_type times = circular_map[index];
                                         ++circular_map[index];
                                         if (times> limit_circular_times){
                                              cout << endl << "Error:" << "circular tail " << token[index] << "  " << "position:" << index << "  circular is more than " << limit_circular_times << " times" << endl;


                                              return false;
                                         }
                                         circular_layers = 1;
                                         if (!array_ram[pointer]){
                                              break;
                                         }
                                         while (circular_layers){
                                              --index;
                                              if (circular_right == token[index])
                                              {
                                                   ++circular_layers;
                                              }
                                              if (circular_left == token[index])
                                              {
                                                   --circular_layers;
                                              }


                                         }
                                         break;


           }
           case less_than:{
                                  if (pointer > 0){

                                       --pointer;
                                  }
                                  else {
                                       cout << "Warnning:" << "less_than" << "    " << "pointer:" << pointer << " " << "position:" << index << endl;
                                       cout << "pointer reset " << fix_ram_size << endl;
                                       pointer = fix_ram_size;

                                  }
                                  break;

           }
           case greater_than:{
                                      if (pointer == fix_ram_size){
                                           cout << "Warnning:" << "greater_than" << "    " << "pointer:" << pointer << " " << "position:" << index << endl;
                                           cout << "pointer reset " << 0 << endl;
                                           pointer = 0;

                                      }
                                      else if (pointer<fix_ram_size){
                                           ++pointer;
                                      }

                                      break;
           }
           case add:{
                          if (pointer>fix_ram_size || pointer<0){
                               cout << "Error:" << "add" << "    " << "pointer:" << pointer << " " << "position:" << index << endl;

                               return false;
                          }
                          ++array_ram[pointer];
                          break;

           }
           case reduce:{
                               if (pointer>fix_ram_size || pointer<0){
                                    cout << "Error:" << "reduce" << "    " << "pointer:" << pointer << " " << "position:" << index << endl;

                                    return false;
                               }
                               --array_ram[pointer];
                               break;
           }
           case input:{
                             if (pointer>fix_ram_size || pointer < 0){
                                  cout << "Error:" << "input" << "    " << "pointer:" << pointer << " " << "position:" << index << endl;

                                  return false;
                             }

                             cin.clear();
                             cin.sync();
                             char tmp;

                             cin.get(tmp);
                             if (tmp == '!'){
                                  cout << "Stop!" << endl;
                                  return false;
                             }
                             array_ram[pointer] = tmp;

                             break;
           }
           case output:{
                               if (pointer > fix_ram_size || pointer < 0){
                                    cout << "Error:" << "output" << "    " << "pointer:" << pointer << endl;

                                    return false;
                               }
                               cout << array_ram[pointer];
                               break;
           }

           case ' ':{
                          break;
           }
           case'r':{
                          break;
           }
           case'n':{
                          break;
           }
           case'#': {
                          cout << "closing" << endl;
                          return 2;
           }
           case'~':{

                          memset(array_ram, 0, (fix_ram_size+1)*sizeof(char));
                          cout << "clear memory" << endl;
                          break;
           }
           case'$':{
                          if (s == false){
                               s = true;
                          }
                          else{
                               s = false;
                          }
                          break;

           }


           case'%':{
                          cout << "point:" << pointer << endl;
                          break;
           }
           case '@':{
                          cin.clear();
                          cin.sync();
                          circular_times_type tmp, limit_circular_times_max = UINT_MAX - 1;

                          cout << "input  circular limit times" << endl;
                          cout << "The number  must be in the range of " << 0 << " to " << limit_circular_times_max << endl;
                          cin >> tmp;
                          if (tmp <= limit_circular_times_max){
                               limit_circular_times = tmp;
                               cout << "limit circular limit times : " << limit_circular_times << endl;
                          }
                          else{
                               cout << "The number is beyond the range of " << 0 << " to " << limit_circular_times_max << endl;
                               cout << "limit circular limit times : " << limit_circular_times << endl;
                          }

                          break;

           }
          

           default:
                if (s == false){
                     cout << "Error: " << token[index] << " is not token " << ' ' << "position:" << index << endl;
                     return false;
                }
                array_ram[pointer] = token[index];
                break;
           }
      }

      return true;

 }