import random
s = """Lorem ipsum dolor sit amet, consectetur adipiscing elit. Integer malesuada lorem at aliquet convallis. Ut porta convallis tincidunt. Aenean ut fermentum arcu, in molestie diam. Mauris purus massa, lobortis sit amet egestas et, cursus a purus. Maecenas consequat orci a leo hendrerit euismod. Sed interdum libero sed nulla sodales tincidunt. Etiam tristique nulla erat, a lobortis sapien sollicitudin sit amet. Vestibulum gravida aliquam ullamcorper. Proin porta magna non ex vulputate, cursus porta magna tristique. Donec porttitor mi vel ex tincidunt, eget aliquet massa condimentum. Fusce a urna vitae tellus suscipit consequat sed eu ipsum. Nunc et justo at dolor venenatis posuere. Aliquam posuere ut justo sed elementum. Etiam tortor augue, malesuada eu laoreet a, suscipit in massa. Praesent porttitor sed ex a faucibus."

Aliquam viverra lacus eget interdum aliquam. Vestibulum finibus dignissim diam nec interdum. Vivamus vulputate in nisl id consequat. Proin bibendum, dolor eget consequat consectetur, metus elit convallis ex, et porta leo mi non mi. Phasellus at ante metus. Cras vulputate, quam sed viverra lobortis, purus libero consequat ligula, eu imperdiet quam augue vel diam. Vivamus placerat gravida lorem sit amet tincidunt. Proin vulputate bibendum nulla et vestibulum. Proin aliquet neque a vulputate iaculis.

Etiam dictum tortor ut dolor congue, in fringilla felis pretium. Nullam pulvinar tempus nulla nec congue. Suspendisse ut dui vel ante facilisis pretium at a tortor. Ut sit amet purus ornare, pretium justo ac, faucibus enim. Donec lectus urna, dapibus vel rhoncus vel, euismod sed est. Sed ut nibh purus. Quisque volutpat venenatis semper. In hac habitasse platea dictumst. Phasellus accumsan, ante et imperdiet porta, purus sem hendrerit diam, eu tincidunt elit orci vel urna. Aliquam condimentum efficitur consequat. Etiam consectetur turpis vel interdum eleifend. Maecenas vulputate libero rhoncus auctor facilisis. In vel lorem et ante faucibus fringilla. Nunc sodales in arcu vel tincidunt. Duis semper, tortor ac venenatis sollicitudin, erat ante placerat mi, ut rhoncus purus orci non ante.

Cras euismod turpis euismod purus vulputate suscipit. Phasellus nunc arcu, commodo a libero a, ultricies lobortis est. Suspendisse sed magna leo. Morbi aliquam viverra metus id pharetra. Interdum et malesuada fames ac ante ipsum primis in faucibus. In lacinia, neque ut pharetra ornare, nibh libero laoreet mauris, eget venenatis sem nunc ut quam. Nulla faucibus justo a quam pulvinar varius. Donec accumsan arcu a laoreet euismod. Donec fringilla ornare varius. Curabitur gravida odio vitae venenatis aliquet. Praesent laoreet, erat nec finibus scelerisque, elit erat laoreet dui, et blandit nisl urna a orci. Praesent gravida, nibh sit amet cursus posuere, ipsum quam ullamcorper leo, eu dapibus augue felis ac tellus. Vestibulum in orci at magna scelerisque commodo.

Ut at nibh ante. Mauris efficitur diam bibendum urna tristique pellentesque quis sit amet nulla. Sed in volutpat ex. Curabitur vitae nibh posuere, consequat erat sed, mollis ante. Morbi consectetur magna a dolor tempor, sed aliquet velit mollis. Suspendisse sit amet quam vehicula, cursus turpis ut, luctus diam. Integer et velit et diam blandit aliquam ac a mi. Sed eu auctor tortor. Lorem ipsum dolor sit amet, consectetur adipiscing elit. Morbi maximus nisi vitae purus luctus sodales. Donec vel tristique diam. In nisl lectus, blandit nec pretium non, mollis sit amet lacus."""

a = list(map(lambda x: x.split(' '), s.split('\n')))

out = []

for i in a:
    out.append([])
    for j in i:
        if random.random() < 0.2:
            out[-1].append(random.choice(["Apple", "apple", "Orange", "orange", "Lemon", "lemon"]))
        else:
            out[-1].append(j)

print('\n'.join(map(' '.join, out)))
