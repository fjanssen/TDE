#include "fann.h"
#include "ImageConverter.h"


int main()
{
	createDataset();

//	const char * learn_file_name = "learn.data";
//	const char * net_file_name = "tde.net";
//    unsigned int layers[3] = {3072, 5000, 43};
//    const float desired_error = (const float) 0.001;
//    const unsigned int max_epochs = 5000;
//    const unsigned int epochs_between_reports = 100;

//    struct fann *ann = fann_create_standard_array(3,layers);
	
    //fann_set_activation_function_hidden(ann, FANN_LINEAR_PIECE_SYMMETRIC);
    //fann_set_activation_function_output(ann, FANN_LINEAR_PIECE_SYMMETRIC);
	//fann_set_training_algorithm(ann,FANN_TRAIN_BATCH);

	
//    fann_train_on_file(ann, learn_file_name, max_epochs, epochs_between_reports, desired_error);
//
//    fann_save(ann, net_file_name);
//
//    fann_destroy(ann);

    return 0;
}
