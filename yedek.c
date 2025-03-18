// void quotes(t_input *input)
// {
//     int i;

//     i = -1;
//     empty_line(input);
//     if (input->isdigit == 0)
//     {
//         input->error = 1;
//         return;
//     }
//     while (input->input[++i])
//     {
//         if (input->input[i] == 34)
//         {
//             input->quotes++;
//             i++;
//             while (input->input[i] != 34 && input->input[i] != '\0')
//                 i++;
//             if (input->input[i] == 34)
//                 input->quotes++;
//         }
//         if (input->input[i] == 39)
//         {
//             input->quotes++;
//             i++;
//             while (input->input[i] != 39 && input->input[i] != '\0')
//                 i++;
//             if (input->input[i] == 39)
//                 input->quotes++;
//         }
//     }
//     if (input->quotes%2!=0)
//         input->error=1;
// }