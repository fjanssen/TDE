#include "fann.h"
#include "ImageConverter.h"
#include "NeuralNetRes.h"

int main()
{
	struct featureList* fList;

	fList = createDataset();
	printf("---sets created---\n");

//	const char * learn_file_name = "../dataset/train.txt";
//	const char * net_file_name = "tde.net";
	unsigned int layers[4] = {3072, 3072, 3072, 43};
	const float desired_error = (const float) 0.001;
	const unsigned int max_epochs = 500;
	const unsigned int epochs_between_reports = 1;

	struct fann *ann = fann_create_standard_array(4,layers);

	fann_train_on_file(ann, "../dataset/train.txt", max_epochs, epochs_between_reports, desired_error);

	fann_save(ann, "tde.net");

	fann_destroy(ann);



	printf("---Learning Finished---\n");

	testNet(fList,43, 3072);

    return 0;
}
